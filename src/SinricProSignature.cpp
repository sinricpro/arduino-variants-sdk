/*
 *  Copyright (c) 2019 Sinric. All rights reserved.
 *  Licensed under Creative Commons Attribution-Share Alike (CC BY-SA)
 *
 *  This file is part of the Sinric Pro (https://github.com/sinricpro/)
 */

#include <WString.h>
#include <ArduinoJson.h>
#include "SinricProSignature.h"
#include "extralibs/Base64/Base64.h"

#if defined (ESP32)
  #include "mbedtls/md.h"
#else
  #include <Seeed_mbedtls.h>
#endif

#include "SinricProNamespace.h"
namespace SINRICPRO_NAMESPACE {

String HMACbase64(const String &message, const String &key) {
  byte hmacResult[32];

  mbedtls_md_context_t ctx;
  mbedtls_md_type_t md_type = MBEDTLS_MD_SHA256;

  mbedtls_md_init(&ctx);
  mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 1);
  mbedtls_md_hmac_starts(&ctx, (const unsigned char*) key.c_str(), key.length());
  mbedtls_md_hmac_update(&ctx, (const unsigned char*) message.c_str(), message.length());
  mbedtls_md_hmac_finish(&ctx, hmacResult);
  mbedtls_md_free(&ctx);
 
  int b64_len = base64_enc_len(32);
  char sigBuf[b64_len + 1];
  base64_encode(sigBuf, (char*) hmacResult, 32);
  sigBuf[b64_len] = 0;  
  return String { sigBuf };
}

String extractPayload(const char *message) {
  String messageStr(message);
  int beginPayload = messageStr.indexOf("\"payload\":");
  int endPayload = messageStr.indexOf(",\"signature\"", beginPayload);
  if (beginPayload >0 && endPayload >0) return messageStr.substring(beginPayload+10, endPayload);
  return "";
}

String calculateSignature(const char* key, String payload) {
  if (payload != "") return HMACbase64(payload, String(key));
  return "";
}

String signMessage(String key, JsonDocument &jsonMessage) {
  if (!jsonMessage.containsKey("signature")) jsonMessage["signature"].to<JsonObject>();
  jsonMessage["signature"]["HMAC"] = calculateSignature(key.c_str(), jsonMessage["payload"]);
  String signedMessageString;
  serializeJson(jsonMessage, signedMessageString);
  return signedMessageString;
}

} // SINRICPRO_NAMESPACE