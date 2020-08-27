import { Message, Field } from "protobufjs/light";

import { pbufferNamespace } from "./namespace";

/**
 * Message used to report health status to the GameLift service.
 *
 * @internal
 */
class ReportHealth extends Message<ReportHealth> {
  /**
   * Health status of the process
   *
   * @internal
   */
  @Field.d(1, "bool", "optional", false)
  public healthStatus: boolean;
}

pbufferNamespace.add(ReportHealth.$type);

export { ReportHealth };
