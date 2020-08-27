import { Message, Field } from "protobufjs/light";

import { pbufferNamespace } from "./namespace";

/**
 * Message from the GameLift server SDK to the GameLift service that the
 * given player is attempting to connect to the game.
 *
 * @internal
 */
class AcceptPlayerSession extends Message<AcceptPlayerSession> {
  /**
   * Identifier for the game session the player is attempting to join.
   *
   * @internal
   */
  @Field.d(1, "string")
  public gameSessionId: string;

  /**
   * Identifier for the player session that's attempting to join the game.
   *
   * @internal
   */
  @Field.d(2, "string")
  public playerSessionId: string;
}

pbufferNamespace.add(AcceptPlayerSession.$type);

export { AcceptPlayerSession };
