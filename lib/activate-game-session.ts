import { Message, Field } from "protobufjs/light";

import { pbufferNamespace } from "./namespace";

import { GameSession } from "./game-session";

/**
 * Message used to signal to the process to begin the attached game session.
 *
 * * This message occurs when the [CreateGameSession API] is utilized and is
 * thus propagated back to the GameLift client process.
 *
 * [CreateGameSession API]: https://docs.aws.amazon.com/gamelift/latest/apireference/API_CreateGameSession.html
 *
 * @internal
 */
class ActivateGameSession extends Message<ActivateGameSession> {
  /**
   * Game session the process needs to begin setting up for.
   *
   * @internal
   */
  @Field.d(1, GameSession)
  public gameSession: GameSession;
}

pbufferNamespace.add(ActivateGameSession.$type);

export { ActivateGameSession };
