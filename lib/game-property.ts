import { Message, Field } from "protobufjs/light";

import { pbufferNamespace } from "./namespace";

/**
 * Set of key-value pairs that contain information about a game session.
 *
 * When included in a game session request, these properties communicate details to
 * be used when setting up the new game session. For example, a game property might
 * specify a game mode, level, or map. Game properties are passed to the game server
 * process when initiating a new game session. For more information, see
 * [the Amazon GameLift Developer Guide's section on creating game sessions].
 *
 * [the Amazon GameLift Developer Guide's section on creating game sessions]: https://docs.aws.amazon.com/gamelift/latest/developerguide/gamelift-sdk-client-api.html#gamelift-sdk-client-api-create
 */
class GameProperty extends Message<GameProperty> {
  /**
   * The game property identifier.
   */
  @Field.d(1, "string")
  key: string;

  /**
   * The game property value.
   */
  @Field.d(2, "string")
  value: string;
}

pbufferNamespace.add(GameProperty.$type);

export { GameProperty };
