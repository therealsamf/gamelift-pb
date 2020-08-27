module.exports = {
  env: {
    node: true,
    es2020: true,
  },
  extends: [
    "eslint:recommended",
    "plugin:@typescript-eslint/recommended",
    "plugin:prettier/recommended",
    "prettier/@typescript-eslint",
  ],
  parser: "@typescript-eslint/parser",
  parserOptions: {
    project: "tsconfig.json"
  },
  plugins: ["@typescript-eslint", "prettier", "eslint-plugin-tsdoc"],
  rules: {
    "tsdoc/syntax": "warn"
  },
};
