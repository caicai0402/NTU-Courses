import Question from '../models/Question'
import Answer from '../models/Answer'


exports.GetContents = async (req, res) => {
  // TODO : get questions from mongodb and return to frontend
  const contents = await Question.find()
  if(contents.length)
    res.status(200).send({ message: 'success', contents: contents })
  else
    res.status(403).send({ message: 'error', contents: [] })
}

exports.CheckAns = async (req, res) => {
  // TODO : get answers from mongodb,
  // check answers coming from frontend and return score to frontend
  const { myAns } = req.body
  const ans = await Answer.find()
  let score = 0
  for (let i = 0; i < ans.length; i++)
    if (myAns[i] === ans[i].answer)
      score += 1
  if (ans.length)
    res.status(200).send({message: 'success', score: score});
  else 
    res.status(403).send({message: 'error', score: -1});
}
