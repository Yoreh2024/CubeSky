#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <event2/buffer.h>
#include "typedefine.h"
#include "codec.h"
#include "common.h"

void clientdata_handler(struct ClientData* data);
void clientdata_reply(struct ClientData* data);
void handler_Handshake(struct ClientData* data);
void handler_EncryptionResponse(struct ClientData* data);
void reply_EncryptionRequest(struct ClientData* data);
void reply_LoginSuccess(struct ClientData* data);