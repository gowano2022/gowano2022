const getQuestionInfo = () => {
  const rootObject = document.querySelector("body > div")
  if (!rootObject) throw new Error("Could not retreive root object");
  let vue = rootObject.__vue__;

  return { 
    roomHash:   vue.$store._vm._data.$$state.game.data.roomHash, 
    playerId:   vue.$store._vm._data.$$state.game.player.playerId, 
    quizID:     vue.$store._vm._data.$$state.game.data.quizId,
    roomCode:   vue.$store._vm._data.$$state.game.data.roomCode,
    questionID: vue.$store._vm._data.$$state.game.questions.currentId,
  };
};

const getQuestionsElement = () => {
  const questionsElem = document.querySelector(
    "body > div > div.root-component > div > div > div > div.page-container.in-quiz > div.screen.screen-game > div.transitioner.transitioner-component > div > div > div > div > div > div.options-container > div"
  );
  if (!questionsElem)
    throw new Error("Unable to retreive questions list element");

  return questionsElem;
};

const highlightAnswers = (question) => {
  const questionsElem = getQuestionsElement();
  const arr = Array.prototype.slice.call(questionsElem.children);

  if (Array.isArray(question.structure.answer) && question.structure.answer.length < 1 && question.structure.options) {
    const answers = question.structure.options.map((option) => option.text).join(" or ");
    alert(answers);

    return;
  }

  arr.filter((e) => {
    if (Array.isArray(question.structure.answer) && question.structure.answer.length > 0) {
      return !(question.structure.answer.some((ansID) => e.__vue__.optionData.actualIndex === ansID));
    } else if(typeof question.structure.answer == "number") {
      if (!e.__vue__) return;
      return e.__vue__.optionData.actualIndex !== question.structure.answer;
    } else {
      console.error("Fail detecting type of question: ", question);
    }
  }).forEach(changeElementOpacity);
}

const changeElementOpacity = (elem) => {
  elem.style.opacity = "20%";
};

const getRoomHash = () => {
  const rootObject = document.querySelector("body > div")
  if (!rootObject) throw new Error("Could not retreive root object");
  let vue = rootObject.__vue__;

  return vue.$store._vm._data.$$state.game.data.roomHash;
}

(async () => {
console.log("%c\n This Script is made by Mohamed A. Gomaa", "color: red;");
let quiz = await (await fetch(`https://quizizz.com/api/main/game/${getRoomHash()}`)).json();
let lastQuestionID = undefined;

setInterval(async () => {
    const questionInfo = getQuestionInfo();
    if (questionInfo.questionID !== lastQuestionID) {
      for (const q of quiz.data.questions) {
        if (questionInfo.questionID === q._id) {
          //console.log({q});
          highlightAnswers(q);
          lastQuestionID = questionInfo.questionID;
        }
      }
    }
  }, 500)
})();
