import { gql } from 'apollo-boost'

const MESSAGES_QUERY = gql`
  query (
    $name: String!
    $talk_to: String!
    $body: String!
    ){
    messages(
      name: $name
      talk_to: $talk_to
      body: $body
      ){
      name
      talk_to
      body
    }
  }
`

const USER_QUERY = gql`
  query (
    $name: String!
    $password: String!
    ){
      users (
        name: $name
        password: $password
      ){
        name
    }
  }
`

const USER_WITH_PASSWORD_QUERY = gql`
  query (
    $name: String!
    $password: String!
    ){
      users_with_password (
        name: $name
        password: $password
      ){
        name
        friends
    }
  }
`

export { MESSAGES_QUERY, USER_QUERY, USER_WITH_PASSWORD_QUERY }