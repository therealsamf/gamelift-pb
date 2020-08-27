import { Message, Field } from "protobufjs/light";

import { pbufferNamespace } from "./namespace";

/**
 * Protocol Buffer message to signal to the GameLift service that the process is
 * ready to host a game session.
 *
 * @internal
 */
class ProcessReady extends Message<ProcessReady> {
  /**
   * List of filepaths that signals which files GameLift should capture and store.
   *
   * @internal
   */
  @Field.d(1, "string", "repeated")
  public logPathsToUpload: string[];

  /**
   * Port that should be given to client connections wanting to connect to
   * the process.
   *
   * @internal
   */
  @Field.d(2, "int32")
  public port: number;

  /**
   * Maximum number of concurrent game sessions on the process.
   *
   * This field is completely undocumented in AWS documentation and I'm not
   * sure if it's a feature that has been implemented yet.
   *
   * @hidden
   */
  @Field.d(3, "int32")
  public maxConcurrentGameSessions = 3;
}

pbufferNamespace.add(ProcessReady.$type);

export { ProcessReady };
