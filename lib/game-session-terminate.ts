import { Message, Field } from "protobufjs/light";

import { pbufferNamespace } from "./namespace";

/**
 * Message that notifies the GameLift service that the server process has shutdown the game session.
 *
 * See [`TerminateGameSession()`] for more information.
 *
 * [`TerminateGameSession()`]: https://docs.aws.amazon.com/gamelift/latest/developerguide/integration-server-sdk-cpp-ref-actions.html#integration-server-sdk-cpp-ref-terminategamesession
 *
 * @internal
 */
class GameSessionTerminate extends Message<GameSessionTerminate> {
  /**
   * Denotes the game session that has been terminated.
   *
   * @internal
   */
  @Field.d(1, "string")
  public gameSessionId: string;
}

pbufferNamespace.add(GameSessionTerminate.$type);

export { GameSessionTerminate };
