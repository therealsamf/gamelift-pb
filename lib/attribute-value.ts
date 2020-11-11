import { MapField, Message, Field } from "protobufjs/light";

import { pbufferNamespace } from "./namespace";

/**
 * Values for use in {@link Player} attribute key-value pairs.
 *
 * This object lets you specify an attribute value using any of the valid
 * data types: string, number, string array, or data map. Each AttributeValue
 * object can use only one of the available properties.
 *
 */
class AttributeValue extends Message<AttributeValue> {
  /**
   * Defines `NONE` type for {@link type} field.
   */
  public static readonly NONE = 0;

  /**
   * Defines `STRING` type for {@link type} field.
   */
  public static readonly STRING = 1;

  /**
   * Defines `DOUBLE` type for {@link type} field.
   */
  public static readonly DOUBLE = 2;

  /**
   * Defines `STRING_LIST` type for {@link type} field.
   */
  public static readonly STRING_LIST = 3;

  /**
   * Defines `STRING_DOUBLE_MAP` type for {@link type} field.
   */
  public static readonly STRING_DOUBLE_MAP = 4;

  /**
   * "Enumeration" that determines the type of attribute value.
   *
   * This isn't a true Protocol Buffer enumeration, just an integer that specifies
   * {@link NONE}, {@link STRING}, {@link DOUBLE}, {@link STRING_LIST}, or
   * {@link STRING_DOUBLE_MAP}.
   */
  @Field.d(1, "int32")
  public type: number;

  /**
   * For single string values.
   *
   * Maximum string length is 100 characters.
   */
  @Field.d(2, "string")
  public S?: number;

  /**
   * For number values, expressed as double.
   */
  @Field.d(3, "double")
  public N?: number;

  /**
   * For a list of up to 10 strings.
   *
   * Maximum length for each string is 100 characters. Duplicate values are
   * not recognized; all occurrences of the repeated value after the first of
   * a repeated value are ignored.
   */
  @Field.d(4, "string", "repeated")
  public SL?: string[];

  /**
   * For a map of up to 10 data string:double pairs.
   *
   * Maximum length for each string value is 100 characters.
   */
  @MapField.d(5, "string", "double")
  public SDM?: { [key: string]: number };
}

pbufferNamespace.add(AttributeValue.$type);

export { AttributeValue };
