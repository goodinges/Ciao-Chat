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

// G propid modid oid*;
// g (moid (oid (N tag | kind tag value)*)*)* 
public class GetPropertyRequest extends Request
{
    public GetPropertyRequest(int moduleId, int objectId, int propId)
    {
        super();
        
        tokens.write('G');
        encodeVaruint(propId);
        tokens.write(']');
        encodeVaruint(moduleId);
        encodeVaruint(objectId);
        tokens.write(']');
        tokens.write(']');
    }
}
