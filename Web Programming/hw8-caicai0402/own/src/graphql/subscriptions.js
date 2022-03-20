import { gql } from 'apollo-boost'

export const MESSAGES_SUBSCRIPTION = gql`
  subscription {
    message {
      mutation
      data {
        name
        talk_to
        body
      }
    }
  }
`