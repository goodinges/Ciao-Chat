#!/usr/bin/python
import sys
if sys.version_info < (2, 6):
    print("must use python 2.6 or greater")
if sys.version_info < (3, 0):
    import Queue
    QUEUE_CLASS = Queue.Queue
    UNICODE_CLASS = unicode
else:
    import queue
    QUEUE_CLASS = queue.Queue
    UNICODE_CLASS = str
import array
import collections
import socket, ssl
import weakref
import threading
import time

def enumof(enum_dictionary, int_key):
    try:
        return enum_dictionary[int_key]
    except:
        return ""

''' Protocol error.
'''
class Error(Exception):
    def __init__(self, msg):
        self.msg = msg

    def __str__(self):
        return self.msg

class ConnectionClosed(Error):
    def __init__(self):
        Error.__init__(self, "Connection closed")

class ResponseError(Error):
    def __init__(self):
        Error.__init__(self, "response error (either invalid in parameters for the call or call isn't allowed or call failed")


class InvalidObjectIdError(Error):
    def __init__(self):
        Error.__init__(self, "object id is 0")

MAX_UINT = 2**32-1

class ScopedLock(object):
    def __init__(self, mutex):
        self.mutex = mutex
        self.mutex.acquire()
    def __del__(self):
        self.mutex.release()

class Cached(object):
    '''Base class for all cached objects.

    Every object is identified by an Id specified as first parameter of the constructor.
    Trying to create two objects with same Id yields the same object. Uses weak references
    to allow the objects to be deleted normally.

    @warning: C{__init__()} is always called, don't use it to prevent initializing an already
    initialized object. Use C{__sk_init_()} instead, it is called only once.
    '''
    def __new__(cls, oid, root, *args, **kwargs):
        if oid == 0:
            return False # invalid id, still return something not to shift parameters
        scl = ScopedLock(root._lock_)
        hashk = cls, oid
        obj = None
        try:
            obj = root._cache_[hashk]
        except KeyError:
            obj = object.__new__(cls)
            root._cache_[hashk] = obj
            if hasattr(obj, '_sk_init_'):
                obj._sk_init_(oid, root, *args, **kwargs)
        return obj
    @staticmethod
    def sk_exists(cls, oid, root):
        if oid == 0: 
            return None # invalid id
        scl = ScopedLock(root._lock_)
        hashk = cls, oid
        try:
            return root._cache_[hashk]
        except KeyError:
            return None
    def __copy__(self):
        return self

class Object(Cached):
    rwlock = threading.Lock()
    def _sk_init_(self, object_id, transport):
        self.transport  = transport
        self.object_id  = object_id
        self.properties = {}
    ''' Retrieve given property id.
    '''
    def _sk_property(self, header, prop_id, cached):
        hit = cached #True
        val = 0
        try:
            self.rwlock.acquire()
            if hit:
                val = self.properties[prop_id]
            self.rwlock.release()
        except KeyError:
            self.rwlock.release()
            hit = False
        if not hit:
            val = self.transport.get(GetRequest(header, self.object_id))
        return val
    def multiget(self, header):
        self.transport.get(GetRequest(header, self.object_id))

''' Connection class that implements Skype IPC. 
'''
class SkypeKit:
    _decoders = {}

    class EventDispatcher(threading.Thread):
        def __init__(self, connection): 
            self.connection = connection
            threading.Thread.__init__(self)
            self.setName('event thread')
        def run(self):
            try:  
                self.connection.process_events(True)
            except:
                self.connection.stop()
                raise

    class ResponseListener(threading.Thread):
        def __init__(self, connection): 
            self.connection = connection
            threading.Thread.__init__(self)
            self.setName('responser listener thread')
        def run(self): 
            try:  
                self.connection._start()
            except:
                self.connection.stop()
                raise

    def __init__(self, apptoken, module_id2classes, 
has_event_thread = True, host = '127.0.0.1', port = 8963, logtransport=False, secure=True, setup=''):
        self.module_id2classes = module_id2classes
        if port != None:
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            sock.setsockopt(socket.SOL_SOCKET, socket.SO_KEEPALIVE, True)
            sock.setsockopt(socket.SOL_TCP, socket.TCP_NODELAY, True)
        else:
            sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
        sock.settimeout(30.5)
        self.pending_requests = {}
        self.pending_gets = collections.deque()
        self.pending_lock = threading.Lock()
        self.encoding_lock = threading.Lock()
        self.decoded = threading.Event()
        self.event_queue = QUEUE_CLASS()
        self._lock_  = threading.Lock()
        self._cache_ = weakref.WeakValueDictionary()
        self.inlog  = False
        self.outlog = False
        if logtransport:
            try:
                self.inlog  = open(logtransport+'_log_in.1', 'wb')
            except IOError:
                self.inlog  = False
            try:
                self.outlog = open(logtransport+'_log_out.1', 'wb')
            except IOError:
                self.outlog = False
        self.stopped = False
        retry = 3
        while retry > 0:
            try:
                if port != None:
                    sock.connect((host, port))
                else:
                    sock.connect('\0'+host)
                retry = -1
            except:
                retry = retry - 1
                if retry == 0:
                    raise
                time.sleep(5)
        if secure:
            cert = ""
            sock = ssl.wrap_socket(sock, server_side=True, certfile=apptoken, ssl_version=ssl.PROTOCOL_TLSv1)
        else:
            f = open(apptoken, 'r')
            cert = f.read()
            f.close() 
        self.socket = sock
        self.read_buffer = ''
        self.handshake(sock, setup, cert)
        if has_event_thread:
            self.event_dispatcher = SkypeKit.EventDispatcher(self)
            self.event_dispatcher.start()
        self.listener = SkypeKit.ResponseListener(self)
        self.listener.start()

    def handshake(self, sock, setup, cert):
        setup = setup.encode('utf-8')
        cert = cert+setup
        req = ('%08x'%len(cert))+cert
        sock.sendall(req)
        if self.outlog:
            try:
                self.outlog.write(req)
            except IOError:
                self.outlog.close()
                self.outlog = False
        if self._read_byte(2) != 'OK': 
            raise ConnectionClosed

    def set_root(self, root):
        self.root = root

    def __del__(self):
        if self.socket != None:
            self.socket.close()
        if self.inlog:
            self.inlog.close() 
        if self.outlog:
            self.outlog.close()
  
    def _read_byte(self, num_bytes_to_read = 1):
        result = self.read_buffer
        while not self.stopped and len(result) < num_bytes_to_read: 
            try:
                read = self.socket.recv(4096)
                if not read:
                    self.stop()
                    raise ConnectionClosed
                result += read
                if self.inlog:
                    try:
                        self.inlog.write(read)
                    except IOError:
                        self.inlog.close()
                        self.inlog = False
            except socket.timeout:
                pass # retry always: if connection is closed we will see it at next read
            except ssl.SSLError as strerror:
                if "timed out" in str(strerror): 
                    pass
                elif not self.stopped:
					self.stop();
					raise 
            except:
                if not self.stopped:
                    self.stop()
                    raise 
        if self.stopped:
            return None
        self.read_buffer = result[num_bytes_to_read:]
        result = result[:num_bytes_to_read]
        return result

    def process_events(self, in_thread = False):
        while in_thread or not self.event_queue.empty():
            event = self.event_queue.get()
            if self.stopped:
                return
            event.dispatch()

    def xcall(self, request):
        ev_resp_here  = threading.Event()
        self.encoding_lock.acquire()
        self.pending_lock.acquire()
        self.pending_requests[request.rid] = ev_resp_here
        self.pending_lock.release()
        req = request.send()
        self.socket.sendall(req)
        if self.outlog:
            try:
                self.outlog.write(req)
            except IOError:
                self.outlog.close()
                self.outlog = False
        self.encoding_lock.release()
        ev_resp_here.wait() # no need to clear: one shot
        if self.stopped:
            raise ConnectionClosed()
        resp = self._decode_parms()
        self.decoded.set()
        return resp

    def multiget(self, header, objects):
        if len(objects) > 0:
            self.get(GetRequest(header, objects))

    def get(self, request):
        ev_resp_here  = threading.Event()
        self.encoding_lock.acquire()
        self.pending_lock.acquire()
        self.pending_gets.append(ev_resp_here)
        self.pending_lock.release()
        req = request.send()
        self.socket.sendall(req)
        if self.outlog:
            try:
                self.outlog.write(req)
            except IOError:
                self.outlog.close()
                self.outlog = False
        self.encoding_lock.release()  # wait until response is here
        ev_resp_here.wait()
        if self.stopped: 
            raise ConnectionClosed()
        # process the response with patching the instances...
        return self._decode_get_response()

    def _decode_get_response(self):
        response  = None
        mresponse = {}
        continue_sign = ','
        count = 0
        while continue_sign == ',':
            modid = self._decode_varuint() # modid 
            while continue_sign == ',':
                oid = self._decode_varuint() # oid
                obj = self.module_id2classes[modid](oid, self)
                if not obj in mresponse: 
                    mresponse[obj] = {} 
                kind = self._read_byte() 
                while kind != ']':
                    propid = self._decode_varuint() # propid
                    if kind != 'N':
                        response = self._decoders[kind](self)
                        mresponse[obj][propid] = response
                        obj.rwlock.acquire()
                        obj.properties[propid] = response
                        obj.rwlock.release()
                        count = count + 1
                    kind = self._read_byte() # ] finish the list
                if kind != ']':
                    raise ResponseError() 
                continue_sign = self._read_byte() 
            if continue_sign != ']':
                raise ResponseError()
            continue_sign = self._read_byte()
        if continue_sign != ']':
            raise ResponseError()
        if self._read_byte() != 'z':
            raise ResponseError()
        self.decoded.set()
        if count > 1:
            response = mresponse
        return response

    def _get_response(self):
        self.pending_lock.acquire()
        self.pending_gets.popleft().set()
        self.pending_lock.release()
    _decoders['g'] = _get_response

    def _decode_varuint(self):
        shift  = 0
        result = 0
        while 1:
            value  = ord(self._read_byte()) & 0xff
            result = result | ((value & 0x7f) << shift)
            shift  = shift + 7
            if not (value & 0x80): break
        return result
    _decoders['u'] = _decode_varuint
    _decoders['O'] = _decode_varuint
    _decoders['e'] = _decode_varuint

    def _decode_varint(self):
        value = self._decode_varuint()
        if not value & 0x1:
            return value >> 1
        return (value >> 1) ^ (~0)
    _decoders['i'] = _decode_varint

    def _decode_true(self):
        return True
    _decoders['T'] = _decode_true

    def _decode_false(self):
        return False
    _decoders['F'] = _decode_false

    def _decode_list(self):
        decoded_list = []
        while True:
            byte = self._read_byte()
            if byte == ']': 
                return decoded_list
            decoder = self._decoders[byte] 
            if decoder:
                decoded_list.append(decoder(self))
    _decoders['['] = _decode_list

    def _decode_binary(self):
        length = self._decode_varuint()
        val = ''
        if length > 0:
            val = self._read_byte(length)
        return val

    def _decode_string(self):
        string = self._decode_binary()
        return string.decode('utf-8', 'ignore')
    _decoders['f'] = _decode_string
    _decoders['B'] = _decode_binary
    _decoders['S'] = _decode_string
    _decoders['X'] = _decode_string

    class Parms(dict):
        def get(self, index, defval = None):
            try:
                return self[index]
            except:
                if defval == None: defval = 0
                return defval

    def _decode_parms(self):
        parms = self.Parms()
        decoder = True
        while decoder != None:
            byte = self._read_byte()
            if self.stopped or byte == 'z': break
            if byte != 'N':
                decoder = self._decoders[byte] 
                tag = self._decode_varuint()
                if decoder:
                    parms[tag] = decoder(self)
            else:
                #print "response error ", self.read_byte() # shall be z
                #self.decoded.set()
                self._read_byte() # z
                self.decoded.set()
                raise ResponseError()
        #self.decoded.set()
        return parms

    class Event(object):
        def __init__(self, transport, modid, target, evid, parms):
            self.module_id = modid
            self.target    = target
            self.event_id  = evid
            self.transport = transport
            self.parms     = parms
            transport.decoded.set()
        def dispatch(self):
            target = self.target
            if self.module_id != 0:
                target = Cached.sk_exists(self.transport.module_id2classes[self.module_id], self.parms[0],  self.transport)
                if target == None:
                    return
            getattr(target, target.event_handlers[self.event_id])(self.parms)

    def _decode_event(self):
        # push the event in the event queue
        modid   = self._decode_varuint()
        target  = self.root
        evid    = self._decode_varuint()
        parms   = self._decode_parms()
        self.event_queue.put(SkypeKit.Event(self, modid, target, evid, parms))
    _decoders['E'] = _decode_event

    class PropertyChange(object):
        def __init__(self, transport, modid, oid, propid, val):
            self.transport = transport
            self.modid  = modid
            self.oid    = oid
            self.propid = propid
            self.val    = val
            transport.decoded.set()
        def dispatch(self):
            obj = Cached.sk_exists(self.transport.module_id2classes[self.modid], self.oid, self.transport)
            if obj == None:
                return
            try:
                propname = obj.propid2label[self.propid]
            except KeyError:
                return
            obj.rwlock.acquire()
            if self.val:
                obj.properties[self.propid] = self.val
            else: 
                try:
                    del obj.properties[self.propid]
                except KeyError:
                    pass
            obj.rwlock.release()
            obj.OnPropertyChange(propname)

    def _decode_property_change(self):
        # push the event in the event queue
        modid  = self._decode_varuint()
        oid    = self._decode_varuint() # obj id
        kind   = self._read_byte()      # prop kind
        propid = self._decode_varuint() # prop id
        val = None # invalidate the value
        if kind != 'N': 
            val = self._decoders[kind](self)
        self._read_byte(4) # ]]]z
        self.event_queue.put(SkypeKit.PropertyChange(self, modid, oid, propid, val))
    _decoders['C'] = _decode_property_change

    def _xcall_response(self):
        rid = self._decode_varuint()
        self.pending_lock.acquire()
        ev_resp_here = self.pending_requests[rid]
        del self.pending_requests[rid]
        self.pending_lock.release()
        ev_resp_here.set()
    _decoders['r'] = _xcall_response

    def _start(self):
        while not self.stopped:
            if self._read_byte(1) == 'Z':
                if self.stopped:
                    return
                cmd = self._read_byte()
                if self.stopped:
                    return
                decoder = self._decoders[cmd]
                if decoder:
                    decoder(self)
                    self.decoded.wait()
                    self.decoded.clear() # shall be done immediatly after set?

    def stop(self):
        if not self.stopped:
            self.stopped = True
            if self.socket: 
                self.socket.shutdown(socket.SHUT_RDWR)
                self.socket.close()
                self.socket = None
            self.decoded.set() # ensure that Listener thread resumes
            self.event_queue.put({}) # ensure that event thread resumes
            for ev_get in self.pending_gets:
                ev_get.set()
            for ev_req in self.pending_requests:
                self.pending_requests[ev_req].set()

class Request:
    ''' Base class for all request that provides the encoding primitives
        and a write buffer
    '''
    def __init__(self):
        self.tokens = array.array('B')
    _encoders = { }

    def _encode_varint(self, number):
        if number >= 0:
            number = number << 1
        else: 
            number = (number << 1) ^ (~0)
        self._encode_varuint(number)
    _encoders['i'] = _encode_varint

    def _encode_varuint(self, number):
        tok = self.tokens
        while 1:
            towrite = number & 0x7f
            number = number >> 7
            if number == 0:
                tok.append(towrite)
                break
            tok.append(0x80|towrite)
    _encoders['u'] = _encode_varuint
    _encoders['e'] = _encode_varuint
    _encoders['o'] = _encode_varuint

    def _encode_objectid(self, val):
        if not val:
            self._encode_varuint(0) 
        else: 
            self._encode_varuint(val.object_id)
    _encoders['O'] = _encode_objectid

    def _encode_string(self, val):
        tok = self.tokens
        if isinstance(val, UNICODE_CLASS):
            val = val.encode('utf-8')
        length = len(val)
        self._encode_varuint(length)
        if length > 0:
            tok.fromstring(val)
    _encoders['S'] = _encode_string
    _encoders['X'] = _encode_string
    _encoders['f'] = _encode_string
    _encoders['B'] = _encode_string

    def add_parm(self, kind, tag, val):
        tok = self.tokens
        if isinstance(val, list):
            tok.append(ord('['))
            self._encode_varuint(tag)
            encoder = self._encoders[kind]
            for elem in val: 
                if kind != 'b':
                    tok.append(ord(kind))
                    encoder(self, elem)
                else:
                    if elem:
                        tok.append(ord('T'))
                    else:
                        tok.append(ord('F'))
            tok.append(ord(']'))
        elif kind != 'b':
            tok.append(ord(kind))
            if tag == 0:
                self.oid = val.object_id
            self._encode_varuint(tag)
            self._encoders[kind](self, val)
        else:
            if val:
                tok.append(ord('T'))
            else:
                tok.append(ord('F'))
            self._encode_varuint(tag)
        return self

class XCallRequest(Request):
    ''' action call request
    '''
    __request_id = 0
    __request_lock = threading.Lock()
    def __init__(self, header, module_id, method_id):
        Request.__init__(self)
        self.tokens.fromstring(header)
        self.module_id = module_id
        self.method_id = method_id
        self.oid       = 0
        XCallRequest.__request_lock.acquire()
        self.rid = XCallRequest.__request_id
        XCallRequest.__request_id = XCallRequest.__request_id + 1
        XCallRequest.__request_lock.release()
        self._encode_varuint(self.rid)
    def send(self):
        tok = self.tokens
        tok.append(ord('z'))
        self.tokens = None
        return tok

class GetRequest(Request):
    ''' get request: support multiple object id, but not heterogeneous object classes in
        one request
    '''
    def __init__(self, header, object_id):
        ''' constructor, takes a preencoded header, and a signle object id or a list of
            object ids
        '''
        Request.__init__(self)
        tok = self.tokens
        tok.fromstring(header)
        if isinstance(object_id, list):
            prefix = ''
            for obj in object_id:
                tok.fromstring(prefix)
                self._encode_varuint(obj.object_id)
                prefix = ','
        else:
            self._encode_varuint(object_id)
        tok.fromstring(']]z')
    def send(self):
        tok = self.tokens
        self.tokens = None
        return tok

