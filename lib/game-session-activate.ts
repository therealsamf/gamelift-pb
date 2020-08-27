import { Message, Field } from "protobufjs/light";

import { pbufferNamespace } from "./namespace";

/**
 * Message from the GameLift server SDK to the GameLift service that the process has
 * received the game session, activated/initialized it, and is ready to receive
 * player sessions.
 *
 * See [ActivateGameSession()] for more details.
 *
 * [ActivateGameSession()]: https://docs.aws.amazon.com/gamelift/latest/developerguide/integration-server-sdk-cpp-ref-actions.html#integration-server-sdk-cpp-ref-activategamesession
 *
 * @internal
 */
class GameSessionActivate extends Message<GameSessionActivate> {
  /**
   * Identifier for the game session that's been activated
   *
   * @internal
   */
  @Field.d(1, "string")
  public gameSessionId: string;

  /**
   * The maximum number of players that can be connected simultaneously to the game
   * session.
   *
   * @hidden
   */
  @Field.d(2, "int32")
  public maxPlayers: string;

  /**
   * Port number for the game session.
   *
   * @hidden
   */
  @Field.d(3, "int32")
  public port: number;
}

pbufferNamespace.add(GameSessionActivate.$type);

export { GameSessionActivate };
