import { Message, Type } from "protobufjs/light";

import { pbufferNamespace } from "./namespace";

/**
 * Message for retrieving the locations of certicates & keys to setup a TLS session.
 *
 * For more information see [GetInstanceCertificate()].
 *
 * [GetInstanceCertificate()]: https://docs.aws.amazon.com/gamelift/latest/developerguide/integration-server-sdk-cpp-ref-actions.html#integration-server-sdk-cpp-ref-getinstancecertificate
 *
 * @internal
 */
@Type.d("GetInstanceCertificate")
class GetInstanceCertificate extends Message<GetInstanceCertificate> {}

pbufferNamespace.add(GetInstanceCertificate.$type);

export { GetInstanceCertificate };
