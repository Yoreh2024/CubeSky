#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include "typedefine.h"
#include "codec.h"
#include "common.h"

void cs_handler(struct ClientData* data);
void handler_Handshake(struct ClientData* data);
void handler_EncryptionResponse(struct ClientData* data);
void reply_EncryptionRequest(struct ClientData* data);
void reply_LoginSuccess(struct ClientData* data);
void handler_LoginAcknowledged(struct ClientData* data);
void reply_RegistryData(struct ClientData* data);
void reply_LoginPlay(struct ClientData* data);
void handler_AcknowledgeFinishConfiguration(struct ClientData* data);
void reply_StatusResponse(struct ClientData* data);
void handler_StatusRequest(struct ClientData* data);
void handler_PingRequest(struct ClientData* data);
void handler_ClientInformation(struct ClientData* data);
void handler_ServerboundPluginMessage(struct ClientData* data);
void reply_ClientboundPluginMessage(struct ClientData* data);
void reply_FinishConfiguration(struct ClientData* data);
void handler_ConfirmTeleportation(struct ClientData* data);
void reply_GameEvent(struct ClientData* data);
void reply_SynchronizePlayerPosition(struct ClientData* data);
void reply_PingResponse(struct ClientData* data);