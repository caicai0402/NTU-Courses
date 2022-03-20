# hw8_simple_chat_with_graphql

Change the simple chat APP in HW#6 to work with GraphQL.data query interface

## Homework Requirements

0. Click on this [link](https://classroom.github.com/a/kTpXaAsP) to access the reference code from GitHub classroom.
```
https://classroom.github.com/a/kTpXaAsP
```

1. Change directory to "own"
```
cd own
```

2. Copy files from HW#6 or HW#7 here. You are advised to write HW#8 on top of them.

3. Change the APIs from RESTful to GraphQL. That is, you are advised to use graphQL-yoga and React-Apollo for backend and frontend services. Besides, for chatting app purpose, you should still use WebSocket instead of simple HTTP requests.

4. Change the frontend (i.e. the chat window) so that:
- When the chat window starts, you can specify who you are in an input box.
- Two users on different windows can chat to each other 
- For each message, you should specify who you want to talk to and then enter the chat message (similar to HW#6)
- Each window with a specific user should subscribe to any incoming message addressed to this user. That is, if someone sends you a message from another window, your subscription should detect that mutation on DB and that message should appear automatically on your window.
- (Optional. This is difficult) Implement the "Clear" button -- that is, when it is pressed, all the message sent by and addressed to this user should be deleted/removed from the database as well as other users' windows.

5. Feel free to modify the DB schema for your purpose.

6. Start the server and client on ports 4000 and 3000, respectively.

7. Use "dotenv-defaults" and make sure ".env" is in .gitignore so that your MONGO_URL won't be revealed.

## Homework Deadline

9pm, Wednesday, 01/06, 2021.

## Homework Review

If you submit homework in time, you should receive 2 other students' homework for review within 24 hours. Please folloing the instruction and fill in your review in this Google form: https://forms.gle/3oYtwd2rq3EHUjHT7

Note that you need to copy your MONGO_URL link to .env file so that you can test other's program with your own DB. As long as you DO NOT change the name and data types of the MessageSchema, you should be able to run the other's queries on your own data.

Review deadline is: 9pm, Monday, 01/11, 2021.

After the deadline of homework review, TA will push others' review back to your repo within 1 week.

## Contact TA and Ric

If you have any question regarding to this homework or this class, please e-mail to: eewebprogramming@googlegroups.com.

You are also welcome to raise the homework related questions on FB group.

