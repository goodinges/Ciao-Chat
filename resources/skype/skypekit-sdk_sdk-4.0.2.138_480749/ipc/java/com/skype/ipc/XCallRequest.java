/**
 * Copyright (C) 2010, Skype Limited
 *
 * All intellectual property rights, including but not limited to copyrights,
 * trademarks and patents, as well as know how and trade secrets contained in,
 * relating to, or arising from the internet telephony software of
 * Skype Limited (including its affiliates, "Skype"), including without
 * limitation this source code, Skype API and related material of such
 * software proprietary to Skype and/or its licensors ("IP Rights") are and
 * shall remain the exclusive property of Skype and/or its licensors.
 * The recipient hereby acknowledges and agrees that any unauthorized use of
 * the IP Rights is a violation of intellectual property laws.
 *
 * Skype reserves all rights and may take legal action against infringers of
 * IP Rights.
 *
 * The recipient agrees not to remove, obscure, make illegible or alter any
 * notices or indications of the IP Rights and/or Skype's rights and
 * ownership thereof.
 */

package com.skype.ipc;

import java.io.IOException;

public class XCallRequest extends Request {
  private static int requestId = 0;

  private int rid;
  private int moduleId;
  private int methodId;

  public XCallRequest(int moduleId, int methodId) throws IOException {
    super();
    this.moduleId = moduleId;
    this.methodId = methodId;
    this.oid = 0;
    tokens.write('R');
    rid = requestId;
    requestId += 1;
//    encodeVaruint(rid); // CAUTION: THIS ORDER SEEMS TO HAVE BEEN CHANGED.
    encodeVaruint(moduleId);
    encodeVaruint(methodId);
    encodeVaruint(rid);
  }

  public int getRid() {
    return rid;
  }

  public int getModuleId() {
    return moduleId;
  }

  public int getMethodId() {
    return methodId;
  }

  public int getOid() {
    return oid;
  }
  
}


