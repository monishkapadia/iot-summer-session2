#include <ArduinoJson.h>

class JsonParser {
  public:
    String question;
    String answers[4];
    
    
    void convertArray(String line) {
      DynamicJsonDocument doc(capacity);
      deserializeJson(doc, line);
      results = doc["results"][0];
      question = results["question"].as<String>();
      answers[0] = results["correct_answer"].as<String>();
      answers[1] = results["incorrect_answers"][0].as<String>();
      answers[2] = results["incorrect_answers"][1].as<String>();
      answers[3] = results["incorrect_answers"][2].as<String>();
      correct_answer = answers[0];
      randomSeed(millis());
      for(int i=0; i<4; i++) {
        randomVal = random(4);
        temp = answers[randomVal];
        answers[randomVal] = answers[i];
        answers[i] = temp;
      }
    }

    String getCorrectAnswer() {
      return correct_answer;
    }

  private:
    int randomVal;
    String temp;
    JsonObject results;
    const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_ARRAY_SIZE(3) + JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(6) + 500;
    String correct_answer;
};
