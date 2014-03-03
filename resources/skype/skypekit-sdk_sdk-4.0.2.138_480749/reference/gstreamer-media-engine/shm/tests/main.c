/*
 * Copyright (c) 2010 Collabora Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "shm-writer.h"
#include "shm-reader.h"

#include <sys/stat.h>

static void
test_shm_writer_init (void)
{
  ShmWriter *writer;

  writer = shm_writer_new ();
  g_assert (SHM_IS_WRITER (writer));

  g_object_unref (writer);
}

static void
test_shm_writer_open_changed_cb (GObject *object,
    GParamSpec *pspec,
    gpointer user_data)
{
  gboolean open;
  int *signalled = (int *)user_data;

  g_object_get (object, "open", &open, NULL);

  if (*signalled == 0)
    g_assert (open);
  else if (*signalled == 1)
    g_assert (!open);
  else
    g_assert_not_reached ();

  ++*signalled;
}

static void
test_shm_writer_open (void)
{
  ShmWriter *writer;
  gboolean open;
  int signalled = 0;

  writer = shm_writer_new ();
  g_assert (SHM_IS_WRITER (writer));

  g_object_get (writer, "open", &open, NULL);
  g_assert (!open);

  g_signal_connect (writer, "notify::open",
      G_CALLBACK (test_shm_writer_open_changed_cb), &signalled);

  g_assert_cmpint (signalled, ==, 0);

  g_assert (shm_writer_open (writer, "/tmp/thishereisatestsocket") != NULL);

  g_assert_cmpint (signalled, ==, 1);

  g_object_get (writer, "open", &open, NULL);
  g_assert (open);

  g_object_unref (writer);

  g_assert_cmpint (signalled, ==, 2);
}

static void
test_shm_writer_close (void)
{
  ShmWriter *writer;
  gboolean open;
  int signalled = 0;

  writer = shm_writer_new ();
  g_assert (SHM_IS_WRITER (writer));

  g_object_get (writer, "open", &open, NULL);
  g_assert (!open);

  g_signal_connect (writer, "notify::open",
      G_CALLBACK (test_shm_writer_open_changed_cb), &signalled);

  g_assert_cmpint (signalled, ==, 0);

  g_assert (shm_writer_open (writer, "/tmp/thishereisatestsocket") != NULL);

  g_assert_cmpint (signalled, ==, 1);

  g_object_get (writer, "open", &open, NULL);
  g_assert (open);

  shm_writer_close (writer);

  g_assert_cmpint (signalled, ==, 2);

  g_object_get (writer, "open", &open, NULL);
  g_assert (!open);

  g_object_unref (writer);

  g_assert_cmpint (signalled, ==, 2);
}

static void
test_shm_writer_mode (void)
{
  ShmWriter *writer;
  int mode;

  writer = shm_writer_new ();

  g_object_set (writer, "mode", S_IRWXU | S_IRWXG, NULL);

  g_object_get (writer, "mode", &mode, NULL);
  g_assert_cmpint (mode, ==, S_IRWXU | S_IRWXG);

  g_object_set (writer, "mode", S_IRWXU, NULL);

  g_object_get (writer, "mode", &mode, NULL);
  g_assert_cmpint (mode, ==, S_IRWXU);

  g_assert (shm_writer_open (writer, "/tmp/thishereisatestsocket") != NULL);

  g_object_set (writer, "mode", S_IRWXU | S_IRWXG, NULL);

  g_object_get (writer, "mode", &mode, NULL);
  g_assert_cmpint (mode, ==, S_IRWXU | S_IRWXG);

  g_object_unref (writer);
}

static void
test_shm_writer_size (void)
{
  ShmWriter *writer;
  gsize size;

  writer = shm_writer_new ();

  g_object_set (writer, "size", 1000, NULL);

  g_object_get (writer, "size", &size, NULL);
  g_assert_cmpint (size, ==, 1000);

  g_object_set (writer, "size", 2000, NULL);

  g_object_get (writer, "size", &size, NULL);
  g_assert_cmpint (size, ==, 2000);

  g_assert (shm_writer_open (writer, "/tmp/thishereisatestsocket") != NULL);

  g_object_set (writer, "size", 1000, NULL);

  g_object_get (writer, "size", &size, NULL);
  g_assert_cmpint (size, ==, 1000);

  g_object_unref (writer);
}

static void
test_shm_writer_send_buffer_no_clients (void)
{
  ShmWriter *writer;

  writer = shm_writer_new ();
  g_assert (SHM_IS_WRITER (writer));

  g_assert (shm_writer_open (writer, "/tmp/thishereisatestsocket") != NULL);

  g_assert_cmpint (shm_writer_send_buffer (writer, "12345", 6), ==, 0);

  g_object_unref (writer);
}

static void
test_shm_reader_init (void)
{
  ShmReader *reader;

  reader = shm_reader_new ();

  g_object_unref (reader);
}

static void
test_shm_reader_connected_changed_cb (GObject *object,
    GParamSpec *pspec,
    gpointer user_data)
{
  gboolean connected;
  int *signalled = (int *)user_data;

  g_object_get (object, "connected", &connected, NULL);

  if (*signalled == 0)
    g_assert (connected);
  else if (*signalled == 1)
    g_assert (!connected);
  else
    g_assert_not_reached ();

  ++*signalled;
}

static void
test_shm_reader_connect (void)
{
  ShmReader *reader;
  ShmWriter *writer;
  gboolean connected;
  int signalled = 0;
  const gchar *path;

  writer = shm_writer_new ();
  g_assert (SHM_WRITER (writer));

  path = shm_writer_open (writer, "/tmp/thishereisatestsocket");
  g_assert (path != NULL);

  reader = shm_reader_new ();

  g_signal_connect (reader, "notify::connected",
      G_CALLBACK (test_shm_reader_connected_changed_cb), &signalled);

  g_object_get (reader, "connected", &connected, NULL);
  g_assert (!connected);

  g_assert (shm_reader_connect (reader, path));

  g_assert_cmpint (signalled, ==, 1);

  g_object_get (reader, "connected", &connected, NULL);
  g_assert (connected);

  g_object_unref (reader);

  g_assert_cmpint (signalled, ==, 2);

  g_object_unref (writer);
}

static void
test_shm_reader_connect_fail (void)
{
  ShmReader *reader;
  gboolean connected;
  int signalled = 0;

  reader = shm_reader_new ();

  g_signal_connect (reader, "notify::connected",
      G_CALLBACK (test_shm_reader_connected_changed_cb), &signalled);

  g_object_get (reader, "connected", &connected, NULL);
  g_assert (!connected);

  g_assert (shm_reader_connect (reader, "/tmp/thishereisatestsocket") == FALSE);

  g_assert_cmpint (signalled, ==, 0);

  g_object_get (reader, "connected", &connected, NULL);
  g_assert (!connected);

  g_object_unref (reader);

  g_assert_cmpint (signalled, ==, 0);
}

static void
test_shm_reader_close (void)
{
  ShmReader *reader;
  ShmWriter *writer;
  gboolean connected;
  int signalled = 0;
  const gchar *path;

  writer = shm_writer_new ();
  g_assert (SHM_WRITER (writer));

  path = shm_writer_open (writer, "/tmp/thishereisatestsocket");
  g_assert (path != NULL);

  reader = shm_reader_new ();

  g_signal_connect (reader, "notify::connected",
      G_CALLBACK (test_shm_reader_connected_changed_cb), &signalled);

  g_object_get (reader, "connected", &connected, NULL);
  g_assert (!connected);

  g_assert (shm_reader_connect (reader, path));

  g_assert_cmpint (signalled, ==, 1);

  g_object_get (reader, "connected", &connected, NULL);
  g_assert (connected);

  shm_reader_disconnect (reader);

  g_assert_cmpint (signalled, ==, 2);

  g_object_get (reader, "connected", &connected, NULL);
  g_assert (!connected);

  g_object_unref (reader);

  g_assert_cmpint (signalled, ==, 2);

  g_object_unref (writer);
}

static void
test_shm_reader_got_data_cb (GObject *object,
    guint len,
    gpointer data,
    gpointer user_data)
{
  g_assert_cmpint (len, ==, 6);
  g_assert_cmpstr (data, ==, "12345");

  g_main_loop_quit ((GMainLoop *)user_data);
}

static gboolean
test_shm_reader_got_data_send_buffer_cb (gpointer user_data)
{
  ShmWriter *writer = SHM_WRITER (user_data);

  g_assert_cmpint (shm_writer_send_buffer (writer, "12345", 6), ==, 1);

  return FALSE;
}

static void
test_shm_reader_got_data (void)
{
  GMainLoop *loop;
  ShmReader *reader;
  ShmWriter *writer;
  const gchar *path;

  loop = g_main_loop_new (NULL, FALSE);

  writer = shm_writer_new ();
  g_assert (SHM_WRITER (writer));

  path = shm_writer_open (writer, "/tmp/thishereisatestsocket");
  g_assert (path != NULL);

  reader = shm_reader_new ();

  g_assert (shm_reader_connect (reader, path));

  g_signal_connect (reader, "got-data",
      G_CALLBACK (test_shm_reader_got_data_cb), loop);

  g_idle_add (test_shm_reader_got_data_send_buffer_cb, writer);

  g_main_loop_run (loop);

  g_object_unref (reader);
  g_object_unref (writer);
  g_main_loop_unref (loop);
}

int
main (int argc, char **argv)
{
  g_type_init ();
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/shm-writer/init", test_shm_writer_init);
  g_test_add_func ("/shm-writer/open", test_shm_writer_open);
  g_test_add_func ("/shm-writer/close", test_shm_writer_close);
  g_test_add_func ("/shm-writer/mode", test_shm_writer_mode);
  g_test_add_func ("/shm-writer/size", test_shm_writer_size);
  g_test_add_func ("/shm-writer/send-buffer-no-clients",
      test_shm_writer_send_buffer_no_clients);

  g_test_add_func ("/shm-reader/init", test_shm_reader_init);
  g_test_add_func ("/shm-reader/connect", test_shm_reader_connect);
  g_test_add_func ("/shm-reader/connect-fail", test_shm_reader_connect_fail);
  g_test_add_func ("/shm-reader/close", test_shm_reader_close);
  g_test_add_func ("/shm-reader/got-data", test_shm_reader_got_data);

  return g_test_run ();
}
