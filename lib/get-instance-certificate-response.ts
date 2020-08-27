import { Message, Field } from "protobufjs/light";

import { pbufferNamespace } from "./namespace";

/**
 * GameLift service response with the locations of the files necessary to
 * setup a TLS secured server.
 *
 * For more information see [GetInstanceCertificate()].
 *
 * [GetInstanceCertificate()]: https://docs.aws.amazon.com/gamelift/latest/developerguide/integration-server-sdk-cpp-ref-actions.html#integration-server-sdk-cpp-ref-getinstancecertificate
 */
class GetInstanceCertificateResponse extends Message<
  GetInstanceCertificateResponse
> {
  /**
   * Filepath to the TLS certificate.
   */
  @Field.d(1, "string")
  public certificatePath: string;

  /**
   * Filepath to the TLS certificate chain.
   */
  @Field.d(2, "string")
  public certificateChainPath: string;

  /**
   * Filepath to the private key for the TLS certificate.
   */
  @Field.d(3, "string")
  public privateKeyPath: string;

  /**
   * Hostname the certificate has been issued for.
   */
  @Field.d(4, "string")
  public hostName: string;

  /**
   * Filepath to the root certificate.
   */
  @Field.d(5, "string")
  public rootCertificatePath: string;
}

pbufferNamespace.add(GetInstanceCertificateResponse.$type);

export { GetInstanceCertificateResponse };
