import { Message, Field } from "protobufjs/light";

import { GameSession } from "./game-session";
import { pbufferNamespace } from "./namespace";

/**
 * Message from the GameLift service that game session has been updated.
 *
 * This message occurs when the [UpdateGameSession API] is utilized and is
 * thus propagated back to the GameLift client process.
 *
 * [UpdateGameSession API]: https://docs.aws.amazon.com/gamelift/latest/apireference/API_UpdateGameSession.html
 */
class UpdateGameSession extends Message<UpdateGameSession> {
  /**
   * New {@link GameSession} property carrying all the new fields.
   */
  @Field.d(1, GameSession)
  public gameSession: GameSession;

  /**
   * Optional string describing the reason for the update.
   */
  @Field.d(2, "string")
  public updateReason?:
    | "MATCHMAKING_DATA_UPDATED"
    | "BACKFILL_FAILED"
    | "BACKFILL_TIMED_OUT"
    | "BACKFILL_CANCELLED";

  /**
   * Ticket ID for the MatchBackfill request.
   */
  @Field.d(3, "string")
  public backfillTicketId: string;
}

pbufferNamespace.add(UpdateGameSession.$type);

export { UpdateGameSession };
