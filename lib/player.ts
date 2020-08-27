import { MapField, Message, Field } from "protobufjs/light";

import { AttributeValue } from "./attribute-value";
import { pbufferNamespace } from "./namespace";

/**
 * Represents a player in matchmaking.
 *
 * When starting a matchmaking request, a player has a player ID, attributes,
 * and may have latency data. Team information is added after a match has
 * been successfully completed.
 */
class Player extends Message<Player> {
  /**
   * Unique ID for the player
   */
  @Field.d(1, "string")
  public playerId: string;

  /**
   * A collection of key:value pairs containing player information for use
   * in matchmaking.
   *
   * Player attribute keys must match the *playerAttributes* used in a
   * matchmaking rule set.
   *
   * Example:
   *
   * ```
   * "PlayerAttributes": { "skill": { "n": 23 }, "gameMode": { "s": "deathmatch" } }
   * ```
   */
  @MapField.d(2, "string", AttributeValue)
  public playerAttributes: { [key: string]: AttributeValue };

  /**
   * Name of team that the player is assigned to in a match.
   *
   * Team names are defined in a matchmaking rule set.
   */
  @Field.d(3, "string")
  public team: string;

  /**
   * Set of values, expressed in milliseconds, indicating the amount of
   * latency that a player experiences when connecting to AWS Regions.
   *
   * If this property is set, FlexMatch considers the match only in Regions
   * for which latency is reported.
   *
   * If a matchmaker has a rule that evaluates player latency, players must
   * report latency in order to be matched. If no latency is reported in
   * this scenario, FlexMatch assumes that no Regions are available to the
   * player and the ticket is not matchable.
   */
  @MapField.d(4, "string", "int32")
  public latencyInMs: { [region: string]: number };
}

pbufferNamespace.add(Player.$type);

export { Player };
