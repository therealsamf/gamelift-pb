import { Message, Field } from "protobufjs/light";

import { pbufferNamespace } from "./namespace";

/**
 * Message from the GameLift service that the server process is being
 * shutdown
 *
 * A server process may be shut down for several reasons: (1) process poor
 * health, (2) when an instance is being terminated during a scale-down
 * event, or (3) when an instance is being terminated due to a
 * [spot-instance interruption].
 *
 * [spot-instance interruption]: https://docs.aws.amazon.com/gamelift/latest/developerguide/spot-tasks.html
 *
 * @internal
 */
class TerminateProcess extends Message<TerminateProcess> {
  /**
   * UNIX epoch denoting the time in seconds or -1 if the process may be
   * terminated at any time.
   *
   * @internal
   */
  @Field.d(1, "int64")
  public terminationTime: number;
}

pbufferNamespace.add(TerminateProcess.$type);

export { TerminateProcess };
