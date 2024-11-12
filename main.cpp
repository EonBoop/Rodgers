#include <dpp/message.h>
#include <dpp/nlohmann/json_fwd.hpp>
#include <iostream>
#include <curl/curl.h>
#include <dpp/dpp.h>
#include <dpp/intents.h>
#include "json.hpp"
#include <ostream>
#include <regex.h>
#include <regex>
#include <csignal>
#include <vector>



//global definitions
dpp::cluster bot("<bot secret here", dpp::i_default_intents | dpp::i_message_content);
bool runBot = 1;
std::map<std::string, std::string> IDMessagePair;

//don't think this is going to need methods???
struct user {
  std::string userID;
  std::string username;
  bool optState;

};

struct message {
  std::string content;
  std::string ID;
  user* sender;
  int length;
};

struct channel {
  std::string channelID;
  std::string name;
  std::vector<message*> conversation;
  int conversationLength;
  bool isActive;
};

std::vector<user*> userPointers;
//a vectore that points to vectors containing series of messages
std::vector <std::vector<message>*> conversations;

//map that holds channel IDs and points to conversations.
std::map <std::string, channel*> channelIDToPointerMap;
std::map <std::string, user*> UserIDToPointerMap;

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
   nlohmann::json json = nlohmann::json::parse("{\"model\": \"gpt-3.5-turbo\",\"messages\": [{\"role\": \"system\",\"content\": \"You are Mx. Rodgers. <bot prompt here>\"},{\"role\": \"user\",\"content\": \"\"}]}\n");
  json["messages"][1]["content"] = input; 
  holdingString=submitCurl(json.dump());

  json = nlohmann::json::parse(holdingString);
  holdingString =  json["choices"][0]["message"]["content"];
  return holdingString;

}

int newMessageHandling (const dpp::message_create_t& event){
  channel* channelPointer = channelIDToPointerMap[event.msg.channel_id.str()];

  return 0;
}

int formatAndSubmit(const dpp::message_create_t& event){
//note to self save off the pointer from the map for my sanity in the future.

  channel* channelPointer = channelIDToPointerMap[event.msg.channel_id.str()];
  std::string holdingString;

  message* newMessage = new message;
  newMessage->content = UserIDToPointerMap[event.msg.author.id.str()]->username + ":" + event.msg.content;
  newMessage->sender = UserIDToPointerMap[event.msg.author.id.str()];

  channelIDToPointerMap[event.msg.channel_id.str()]->conversation.push_back(newMessage);
  //compile the conversation into into the holding string
  for (int i = 0; i < channelIDToPointerMap[event.msg.channel_id.str()]->conversation.size(); i++) {
    holdingString = holdingString + "\n" + channelIDToPointerMap[event.msg.channel_id.str()]->conversation[i]->content;
  }
  //add prompt for Rodgers
  holdingString += "\nMx. Rodgers: ";//TO DO replace with user* format

  //manage our conversation
  channelIDToPointerMap[event.msg.channel_id.str()]->conversationLength =  channelIDToPointerMap[event.msg.channel_id.str()]->conversationLength + newMessage->content.length();
  //minimum context = the max length of one discord message (920), plus the maximum length of a username + 2
 //If the bot sends a message with more than 2000 characters it will delete all our context... 
  while (channelPointer->conversationLength > 1000) {
    channelPointer->conversationLength = channelPointer->conversationLength - channelPointer->conversation[0]->content.length();
    std::cout << channelPointer->conversation[0] << std::endl;
    delete channelPointer->conversation[0];
    channelPointer->conversation.erase(channelPointer->conversation.begin());
  }

  std::cout << "\nConversation length: " << channelPointer->conversationLength;
  //shove it into the bot
  std::cout << "\ntext to bot : " << holdingString << std::endl;
  holdingString = JSONFuckery(holdingString);
 /* 
  //solve that problem with the newlines not working
  size_t found = holdingString.find("\\n");
  while (found != std::string::npos) {
      holdingString.replace(found, 2, "\n");
      found = holdingString.find("\\n", found + 1);
  } 
*/
  std::cout << "\ntext from bot : " << holdingString << std::endl;
  //reply to our event
  while(holdingString.length() > 920){
    message* botMessage = new message;
    botMessage->content = "\nMx. Rodgers: " + holdingString.substr(0,920); //TO DO replace with user* format
    //botMessage->sender = bot.me.id; gonna put a pin in that. need a user pointer for our bot.
    channelPointer->conversation.push_back(botMessage); 
    
    event.reply(holdingString.substr(0,920),true);
    holdingString.erase(0,920);
    }   
  event.reply(holdingString);
  
 //manage our bots message within the conversation 
  message* botMessage = new message;
  botMessage->content = "\nMx. Rodgers: " + holdingString; //TO DO replace with user* format
  //botMessage->sender = bot.me.id; gonna put a pin in that. need a user pointer for our bot.
  channelPointer->conversation.push_back(botMessage);

  return 0;
}

int cleanConversation(std::vector<message*>& conversation){
  for (int i = 0; i < conversation.size(); i++) {
    delete conversation[i];
  }
  return 0;
}


void activateChannel(const std::string& rawdata) {
  
nlohmann::json rawEventData = nlohmann::json::parse(rawdata);
  //std::cout << rawEventData.dump(4) << std::endl; 
  channelIDToPointerMap[rawEventData["d"]["channel_id"]] = new channel;
  channelIDToPointerMap[rawEventData["d"]["channel_id"]]->name = rawEventData["d"]["channel"]["name"];
  channelIDToPointerMap[rawEventData["d"]["channel_id"]]->isActive = 1;
  channelIDToPointerMap[rawEventData["d"]["channel_id"]]->conversationLength = 0;
  std::cout << "Channel " << channelIDToPointerMap[rawEventData["d"]["channel_id"]]->name <<" activated." << std::endl;
  return;
}


int unregisterChannel(const std::string& rawdata){
  //core dump somwhere in here, guessing clean conversation when we need to start deleting messages?
  nlohmann::json rawEventData = nlohmann::json::parse(rawdata);
  if (channelIDToPointerMap[rawEventData["d"]["channel_id"]] == 0) return 0;
  std::cout << "De-activating channel " << channelIDToPointerMap[rawEventData["d"]["channel_id"]]->name;
  cleanConversation(channelIDToPointerMap[rawEventData["d"]["channel_id"]]->conversation);
  channelIDToPointerMap[rawEventData["d"]["channel_id"]]->conversationLength = 0;
  channelIDToPointerMap[rawEventData["d"]["channel_id"]]->isActive = 0;

  return 0;
}


int registerUser(const std::string& rawdata){

  nlohmann::json rawEventData = nlohmann::json::parse(rawdata);
  //std::cout << rawEventData.dump(4) << std::endl; 
  UserIDToPointerMap[rawEventData["d"]["member"]["user"]["id"]] = new user;
  UserIDToPointerMap[rawEventData["d"]["member"]["user"]["id"]]->username = rawEventData["d"]["member"]["nick"];
  UserIDToPointerMap[rawEventData["d"]["member"]["user"]["id"]]->optState = 1;
  std::cout << "Registering user: " << UserIDToPointerMap[rawEventData["d"]["member"]["user"]["id"]]->username << std::endl;

  return 0;
}


int unRegisterUser(const std::string& rawdata){

  nlohmann::json rawEventData = nlohmann::json::parse(rawdata);
  if (UserIDToPointerMap[rawEventData["d"]["member"]["user"]["id"]] == 0) return 0;
  UserIDToPointerMap[rawEventData["d"]["member"]["user"]["id"]]->optState = 0;
  std::cout << "Unregistering User" << UserIDToPointerMap[rawEventData["d"]["member"]["user"]["id"]]->username << std::endl;

  return 0;
}

int conversationHandling(const dpp::message_create_t& event) {
  //check to see if we should process the message
  //this is also  where we guard against uninitialized pointers
  if ((UserIDToPointerMap[event.msg.author.id.str()] == 0) || (channelIDToPointerMap[event.msg.channel_id.str()] == 0)) return 0;
  if ((UserIDToPointerMap[event.msg.author.id.str()]->optState == 0) || (channelIDToPointerMap[event.msg.channel_id.str()]->isActive == 0)) return 0;
  if (event.msg.author.id == bot.me.id) return 0;
  
  formatAndSubmit(event);

  return 0;
}

int cleanMemory() {
  std::cout << "cleaning memory" << std::endl;

  for (auto& [channelID,pointer] : channelIDToPointerMap) {
    if (pointer !=0) {
      std::cout <<"Deleting channel " << pointer->name << std::endl;
      cleanConversation(pointer->conversation);
      delete pointer;
    }
  }

  for (auto& [userID,pointer] : UserIDToPointerMap) {
    if (pointer !=0) {
      delete pointer;
      std::cout << "Deleting user " << pointer->username << std::endl;;
    }
  }

  return 0;
}

void signal_handler(int signum) {
    //if kill signal is recieved, shutdown the bot first
    cleanMemory();
    bot.shutdown();
    runBot = 0;
    exit(signum); 
}
  
int main(void){
 // std::map<std::string, std::string> IDMessagePair;

    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);
    bot.on_log(dpp::utility::cout_logger());

    bot.on_ready([](const dpp::ready_t& event) {
      if (dpp::run_once<struct register_bot_commands>()) {
         //bot.global_bulk_command_delete();
         bot.global_command_create(dpp::slashcommand("activate", "Activate bot in this channel", bot.me.id));
         bot.global_command_create(dpp::slashcommand("murder", "De-activate bot in this channel", bot.me.id));
         bot.global_command_create(dpp::slashcommand("optIn", "Register your user ID and opt in", bot.me.id));
         bot.global_command_create(dpp::slashcommand("optOut", "De-register your user ID and opt out", bot.me.id));
    }
  });
    bot.on_slashcommand([](const dpp::slashcommand_t& event) {
      if (event.command.get_command_name() == "activate") {
          event.reply("HA, imagine being such a bottom you tried to turn me on.");
          std::cout << event.raw_event << std::endl;
          activateChannel(event.raw_event);
      }     
      if (event.command.get_command_name() == "murder"){
          event.reply("NNNNNOOOOO");
          unregisterChannel(event.raw_event);
      }
      if (event.command.get_command_name() == "optin"){
          event.reply("HOLY SHIT HOLY SHIT I'M SO EXCITED TO MEET YOU : D");
          registerUser(event.raw_event);
      };
      if (event.command.get_command_name() == "optout"){
          event.reply(";-; oh... ok I get it.");
          unRegisterUser(event.raw_event);
          unRegisterUser(event.raw_event);
      };
  });
  /* The event is fired when the bot detects a message in any server and any channel it has access to. */
	  bot.on_message_create([](const dpp::message_create_t& event) {
       conversationHandling(event); 

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
             /* formatAndSubmit(event); //this should be changed so that we just pass in the event and let formatAndSubmit to the reply.
              int textLength = botText.length();
              if (textLength > 920) {
              while (botText.length()>920){
	              event.reply(botText.substr(0, 920), true);
                botText.erase(0,920);
                        }
              //event.reply(botText); Was that it????
             	        }
              else event.reply(botText,true);*/
    }
	    });

  //bot.start(dpp::st_wait);
  bot.start(dpp::st_return);
  while(runBot);
  return 0;
}/*
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

*/
