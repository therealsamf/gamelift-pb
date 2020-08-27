import { Message, Field } from "protobufjs/light";

import { pbufferNamespace } from "./namespace";

/**
 * This data type is used to cancel a matchmaking backfill request.
 *
 * The information is communicated to the GameLift service in a
 * [`StopMatchBackfill()`] call.
 *
 * [`StopMatchBackfill()`]: https://docs.aws.amazon.com/gamelift/latest/developerguide/integration-server-sdk-cpp-ref-actions.html#integration-server-sdk-cpp-ref-stopmatchbackfill
 */
class StopMatchmakingRequest extends Message<StopMatchmakingRequest> {
  /**
   * Unique identifier of the backfill request ticket to be cancelled.
   */
  @Field.d(1, "string")
  public ticketId: string;

  /**
   * Unique game session identifier associated with the request being
   * cancelled.
   */
  @Field.d(2, "string")
  public gameSessionArn: string;

  /**
   * Unique identifier of the matchmaker this request was sent to.
   */
  @Field.d(3, "string")
  public matchmakingConfigurationArn: string;
}

pbufferNamespace.add(StopMatchmakingRequest.$type);

export { StopMatchmakingRequest };
