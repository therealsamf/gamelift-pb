
terraform {
  backend "s3" {
    bucket         = "gamelift-pb-terraform"
    key            = "terraform.tfstate"
    region         = "us-east-1"

    dynamodb_table = "gamelift-pb-terraform"

    profile                 = "backend"
    shared_credentials_file = "credentials"
  }
}

provider "aws" {
  alias      = "site_creator"

  region     = "us-east-1"
  access_key = var.site_creator_access_key
  secret_key = var.site_creator_secret_key
}

module "site" {
  source = "git@github.com:therealsamf/terraform-docs-module.git"

  project_name = "gamelift-pb"
  bucket       = "kontest-docs-gamelift-pb"

  providers = {
    aws = aws.site_creator
  }
}
