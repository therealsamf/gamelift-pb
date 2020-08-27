import { Message, Field } from "protobufjs/light";

import { pbufferNamespace } from "./namespace";

/**
 * Message from the GameLift server SDK to the GameLift service that the
 * given player has left the game and their session is open.
 *
 * @internal
 */
class RemovePlayerSession extends Message<RemovePlayerSession> {
  /**
   * Identifier for the game session the player has left.
   *
   * @internal
   */
  @Field.d(1, "string")
  public gameSessionId: string;

  /**
   * Identifier for the player session that has left to join the game.
   *
   * @internal
   */
  @Field.d(2, "string")
  public playerSessionId: string;
}

pbufferNamespace.add(RemovePlayerSession.$type);

export { RemovePlayerSession };
