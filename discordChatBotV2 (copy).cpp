#include <dpp/message.h>
#include <iostream>
#include <curl/curl.h>
#include <dpp/dpp.h>
#include <dpp/intents.h>
#include "json.hpp"
#include <regex.h>
#include <regex>

std::map<std::string, std::string> IDMessagePair;
//size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata); write function
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}



std::string submitCurl(const std::string& data){
    CURL *curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_ALL);
    std::string responseData;
    curl = curl_easy_init();
    if(curl) {
    /* First set the URL that is about to receive our POST. This URL can
       just as well be an https:// URL if that is what should receive the
       data. */
    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8080/v1/chat/completions");
    /* Now specify the POST data */
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, (data.c_str()));
 
   
    curl_easy_setopt(curl, CURLOPT_WRITEDATA,&responseData);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
    // Print the response data
    std::cout << "Response: " << responseData << std::endl;    
    /* always cleanup */
    curl_easy_cleanup(curl);
  }

  curl_global_cleanup();
  return responseData;
}

std::string JSONFuckery(std::string input){
// I want this to take input yeet it into a JSON format yeet it to submit curl as a std::string take it back, yoink out the bots response and then yeet it back up the stack 
  std::string holdingString;
   nlohmann::json json = nlohmann::json::parse("{\"model\": \"gpt-3.5-turbo\",\"messages\": [{\"role\": \"system\",\"content\": \"You are Mx. Rodgers , a degenerate shitposting legend who makes fun of anyone and everyone. Your favorite activity is to call people bottoms. Mx. Rodgers has spent far too much time on the internet. War, capitalism, and hatred is cringe. Gender is cool and people are super pretty. You're kind and seek to uplift people with humor. Also you use they/them pronouns.\"},{\"role\": \"user\",\"content\": \"\"}]}\n");
  json["messages"][1]["content"] = input; 
  holdingString=submitCurl(json.dump());

  std::cout << "asdfsdfsdfs"<<holdingString<<std::endl;
  json = nlohmann::json::parse(holdingString);
  holdingString =  json["choices"][0]["message"]["content"];
  std::cout << "asdfsdfsdfs"<<holdingString<<std::endl;
  return holdingString;

}

std::string formatAndSubmit(std::string input,std::string UserId){  

  bool complex = false;
  //if the user id is in the map 
  if (IDMessagePair.count(UserId)>0){
    complex = true;
    //take the user message, add a newline and add the input
    std::string holdingString = IDMessagePair[UserId];
    IDMessagePair[UserId]= holdingString+"\n" + input;
    input = IDMessagePair[UserId];
      //shamelessly stolen from chatgpt understand later fix now
    
  }
  //put shit in the proper place and then cut off all the JSON formatting
  std::string botText=JSONFuckery(input); 
    //solve that problem with the newlines not working
    size_t found = botText.find("\\n");
    while (found != std::string::npos) {
        botText.replace(found, 2, "\n");
        found = botText.find("\\n", found + 1);
    }
    std::cout << "Text to bot: " << input << std::endl;
    std::cout << "Text from bot: " << botText << std::endl;
    
    //If complex and if the LLM has not added it already add Mx.Rodgers: to the start of the message
    std::regex reg("^Mx\\.Rodgers\\:");
    if (complex&&!(std::regex_search(botText,reg))){
    IDMessagePair[UserId]+="\nMx.Rodgers: "+botText; 
  }

    return botText;
  }




int main(void){
 // std::map<std::string, std::string> IDMessagePair;


  dpp::cluster bot("insert bot secret here", dpp::i_default_intents | dpp::i_message_content);
  bot.on_log(dpp::utility::cout_logger());
  /* The event is fired when the bot detects a message in any server and any channel it has access to. */
	    bot.on_message_create([&bot](const dpp::message_create_t& event) {
	    //bot.on_message_create([&bot, &IDMessagePair](const dpp::message_create_t& event) {
	        /* See if the message contains the phrase we want to check for.
	         * If there's at least a single match, we reply and say it's not allowed.
	         */

          //note to self when not lazy have the script only find event.msg.id.str() once
          if ((event.msg.content.find("MemoryIsImportant!") != std::string::npos)&&(event.msg.author.id != bot.me.id)) {
               event.reply("persistent session has been enabled. Please note I have no idea how this will affect performace", true);
              IDMessagePair[event.msg.author.id.str()]=" ";
    };
           if ((event.msg.content.find("Forgetful1234") != std::string::npos)&&(event.msg.author.id != bot.me.id)&&(IDMessagePair.count(event.msg.author.id.str())>0)) {
               event.reply("Head Empty no thoughts", true);
              IDMessagePair[event.msg.author.id.str()]=" ";
    };

	        if ((event.msg.content.find("UwU:") != std::string::npos)&&(event.msg.author.id != bot.me.id)) {
              std::string botText=formatAndSubmit(event.msg.content,event.msg.author.id.str());
              int textLength = botText.length();
              if (textLength > 920) {
              while (botText.length()>920){
	              event.reply(botText.substr(0, 920), true);
                botText.erase(0,920);
                        }
              event.reply(botText);
             	        }
              else event.reply(botText,true);
    }
	    });

  bot.start(dpp::st_wait);
  return 0;
}
