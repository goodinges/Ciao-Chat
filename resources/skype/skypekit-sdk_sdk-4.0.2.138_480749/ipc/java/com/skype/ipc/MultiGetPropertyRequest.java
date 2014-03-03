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

import java.util.Collection;
import java.util.Iterator;

//G propid modid oid*;
//g (moid (oid (N tag | kind tag value)*)*)* 

public class MultiGetPropertyRequest extends Request
{
    public MultiGetPropertyRequest(int moduleId, Collection<Integer> objectId, Collection<Integer> propId)
    {
        super();
        
        Iterator<Integer> propidi = propId.iterator();
        Iterator<Integer> oidi = objectId.iterator();
        tokens.write('G');
        while (true) {
            encodeVaruint(propidi.next());
            if (propidi.hasNext()) {
                tokens.write(',');
            }
            else {
                break;
            }
        }
        tokens.write(']');
        encodeVaruint(moduleId);
        while (true) {
            encodeVaruint(oidi.next());
            if (oidi.hasNext()) {
                tokens.write(',');
            }
            else {
                break;
            }
        }
        tokens.write(']');
        tokens.write(']');
    }

}
