import { Message, Field } from "protobufjs/light";

import { pbufferNamespace } from "./namespace";

/**
 * Properties describing a player session.
 *
 * Player session objects are created either by creating a player session for
 * a specific game session, or as part of a game session placement. A player
 * session represents either a player reservation for a game session (status
 * `RESERVED`) or actual player activity in a game session (status `ACTIVE`).
 * A player session object (including player data) is automatically passed to
 * a game session when the player connects to the game session and is
 * validated.
 *
 * When a player disconnects, the player session status changes to
 * `COMPLETED`. Once the session ends, the player session object is retained
 * for 30 days and then removed.
 *
 * @internal
 */
class PlayerSession extends Message<PlayerSession> {
  /**
   * A unique identifier for a player session.
   *
   * @internal
   */
  @Field.d(1, "string")
  public playerSessionId: string;

  /**
   * A unique identifier for a player that is associated with this player
   * session.
   *
   * @internal
   */
  @Field.d(2, "string")
  public playerId: string;

  /**
   * A unique identifier for the game session that the player session is
   * connected to.
   *
   * @internal
   */
  @Field.d(3, "string")
  public gameSessionId: string;

  /**
   * A unique identifier for a fleet that the player's game session is
   * running on.
   *
   * @internal
   */
  @Field.d(4, "string")
  public fleetId: string;

  /**
   * IP address of the instance that is running the game session.
   *
   * When connecting to a Amazon GameLift game server, a client needs to
   * reference an IP address (or {@link dnsName | DNS name}) and port number.
   *
   * @internal
   */
  @Field.d(5, "string")
  public ipAddress: string;

  /**
   * Current status of the player session.
   *
   * Possible player session statuses include the following:
   *
   *  * **RESERVED**: The player session request has been received, but the
   * player has not yet connected to the server process and/or been validated.
   *  * **ACTIVE**: The player has been validated by the server process and
   * is currently connected.
   *  * **COMPLETED**: The player connection has been dropped.
   *  * **TIMEDOUT**: A player session request was received, but the player
   * did not connect and/or was not validated within the timeout limit
   * (60 seconds).
   *
   * @internal
   */
  @Field.d(6, "string")
  public status: "RESERVED" | "ACTIVE" | "COMPLETED" | "TIMEDOUT";

  /**
   * Time stamp indicating when this data object was created. Format is a
   * number expressed in Unix time as milliseconds.
   *
   * @internal
   */
  @Field.d(7, "int64")
  public creationTime: number;

  /**
   * Time stamp indicating when this data object was terminated. Format is a
   * number expressed in Unix time as milliseconds.
   *
   * @internal
   */
  @Field.d(8, "int64")
  public terminationTime: number;

  /**
   * Port number for the game session. To connect to a Amazon GameLift server
   * process, an app needs both the IP address and port number.
   *
   * @internal
   */
  @Field.d(9, "int32")
  public port: number;

  /**
   * Developer-defined information related to a player. Amazon GameLift does
   * not use this data, so it can be formatted as needed for use in the game.
   *
   * @internal
   */
  @Field.d(10, "string")
  public playerData: string;

  /**
   * DNS identifier assigned to the instance that is running the game
   * session. Values have the following format:
   *
   * * TLS-enabled fleets:
   * `<unique identifier>.<region identifier>.amazongamelift.com`.
   * * Non-TLS-enabled fleets:
   * `ec2-<unique identifier>.compute.amazonaws.com`. (See
   * [Amazon EC2 Instance IP Addressing].)
   *
   * When connecting to a game session that is running on a TLS-enabled
   * fleet, you must use the DNS name, not the IP address.
   *
   * [Amazon EC2 Instance IP Addressing]: https://docs.aws.amazon.com/AWSEC2/latest/UserGuide/using-instance-addressing.html#concepts-public-addresses
   *
   * @internal
   */
  @Field.d(11, "string")
  public dnsName: string;
}

pbufferNamespace.add(PlayerSession.$type);

export { PlayerSession };
