import { Message, Field } from "protobufjs/light";

import { pbufferNamespace } from "./namespace";

/**
 * Message to notify the GameLift service the game session can no longer
 * accept players or that it wants to accept some more.
 *
 * See [`UpdatePlayerSessionCreationPolicy()`] for more information.
 *
 * [`UpdatePlayerSessionCreationPolicy()`]: https://docs.aws.amazon.com/gamelift/latest/developerguide/integration-server-sdk-cpp-ref-actions.html#integration-server-sdk-cpp-ref-updateplayersessioncreationpolicy
 *
 * @internal
 */
class UpdatePlayerSessionCreationPolicy extends Message<
  UpdatePlayerSessionCreationPolicy
> {
  /**
   * Denotes the game session whose player session creation policy is being updated.
   *
   * @internal
   */
  @Field.d(1, "string")
  public gameSessionId: string;

  /**
   * String value indicating whether the game session accepts new players.
   *
   * Valid values include:
   *
   *  * **ACCEPT_ALL**: Accept all new player sessions.
   *  * **DENY_ALL**: Deny all new player sessions.
   *
   * @internal
   */
  @Field.d(2, "string")
  public newPlayerSessionCreationPolicy: "ACCEPT_ALL" | "DENY_ALL";
}

pbufferNamespace.add(UpdatePlayerSessionCreationPolicy.$type);

export { UpdatePlayerSessionCreationPolicy };
