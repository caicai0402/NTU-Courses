# hw4_calculator_in_react

Implement an iPhone-like calculator with React.js

## Get started

0. Click on this [link](https://classroom.github.com/a/XogeEm2z) to access the reference code from GitHub classroom.
```
https://classroom.github.com/a/XogeEm2z
```

1. Change directory to "own"
```
cd own
```

2. Now in "own" directory, you first install React related files by the following command:
```
yarn
```
or
```
npm install
```

3. Test if React is installed correctly by:
```
yarn start
```
or
```
npm start
```

You should see an incomplete Calculator like this:

![](https://i.imgur.com/lYvbsyw.png)

And a complete layout should look like this (No worry about "AC" or "C" for reset. You can just implement one of them):

![](https://i.imgur.com/nWglQ28.png)

4. Now you can start coding. Please follow the CSS styles as specified in "src/styles.css". Of course, if you like to change it to a better outlook, please feel free to do so. However, please DO NOT change the class names in order NOT to break the tests.

5. The top React component is "class CalcApp" and is stored in "containers/CalcApp.js". It's partially implemented. You should complete the layout for the missing buttons. Please note that the number '0' should be twice as big and its class should be "bigger-btn".

6. Place the React components in "src/components" directory. We have provided a "CalcButton.js" as an example.

7. Feel free to change the reference code to React Hooks if you like. However, please keep the class names for testing purpose.

8. You can type 'npm test' to test your program.

## Calculator Functionalities

- Basically, follow the functionality of a calculator in your cell phone (not necessarily iphone). For example, press '3 + 6' and you should see '6' on the screen, and press '3 + - 1 =' and you should see '2'.

- When 'AC' is pressed, you should reset the 'state' and the number should reset to '0'.

- You don't need to implement sign (+/-), percentage (%), and floating numbers. Of course, it's optional and feel free to challenge it.

## Homework Deadline

9pm, Monday, 11/09, 2020.

## Homework Review

If you submit homework in time, you should receive 2 other students' homework for review within 24 hours. Please folloing the instruction and fill in your review in this Google form: https://forms.gle/3pDh1cDPDsgN6iEz9

Review deadline is: 9pm, Monday, 11/16, 2020.

After the deadline of homework review, TA will push others' review back to your repo within 1 week.

## Contact TA and Ric

If you have any question regarding to this homework or this class, please e-mail to: eewebprogramming@googlegroups.com.

You are also welcome to raise the homework related questions on FB group.

