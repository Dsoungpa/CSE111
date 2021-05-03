#include "commands.h"
#include "debug.h"

command_hash cmd_hash {
   {"cat"   , fn_cat   },
   {"cd"    , fn_cd    },
   {"echo"  , fn_echo  },
   {"exit"  , fn_exit  },
   {"ls"    , fn_ls    },
   {"lsr"   , fn_lsr   },
   {"make"  , fn_make  },
   {"mkdir" , fn_mkdir },
   {"prompt", fn_prompt},
   {"pwd"   , fn_pwd   },
   {"rm"    , fn_rm    },
   {"rmr"   , fn_rmr   },
};

command_fn find_command_fn (const string& cmd) {
   // Note: value_type is pair<const key_type, mapped_type>
   // So: iterator->first is key_type (string)
   // So: iterator->second is mapped_type (command_fn)
   DEBUGF ('c', "[" << cmd << "]");
   const auto result = cmd_hash.find (cmd);
   if (result == cmd_hash.end()) {
      throw command_error (cmd + ": no such command");
   }
   return result->second;
}

command_error::command_error (const string& what):
            runtime_error (what) {
}

int exit_status_message() {
   int status = exec::status();
   cout << exec::execname() << ": exit(" << status << ")" << endl;
   return status;
}

void fn_cat (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_cd (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   // Just cd check
   if(words.size() == 1){
      state.setCwd(state.getRoot());
      return;
   }

   wordvec splitWords = split(words[1], "/");

   for(int i = 0; i < splitWords.size(); i++){
      if(state.getCwd()->getContent()->
      getPtr(splitWords[i]) == nullptr){
         cout << "Error: No such directory!" << endl;
      }
      else{
         inode_ptr curr = state.getCwd()->
         getContent()->getPtr(splitWords[i]);
         state.setCwd(curr);
      }
   }
}

bool checkIn(inode_state& state, const wordvec& words){
   if(words.size() == 1){
      return true;
   }
   wordvec splitWords = split(words[1], "/");

   for(int i = 0; i < splitWords.size(); i++){
      if(state.getCwd()->getContent()->
      getPtr(splitWords[i]) == nullptr){
         return false;
      }
   }
   return true;
}

void fn_echo (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   cout << word_range (words.cbegin() + 1, words.cend()) << endl;
}


void fn_exit (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   throw ysh_exit();
}

void fn_ls (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   inode_ptr cD = state.getCwd();
   int size = cD->getContent()->getDirent().size();
   map<string, inode_ptr>::iterator it;
   
   for(it = cD->getContent()->getDirent().begin(); 
      it != cD->getContent()->getDirent().end(); it++){
      cout << it->first << endl;
   }
}

void fn_lsr (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_make (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_mkdir (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   inode_ptr saveCurr = state.getCwd();
   string temp = words[1];

   wordvec s = split(words[1], "/");
   if(s.size() > 1){
      while(temp.back() != '/'){
      temp.pop_back();
      }
      temp.pop_back(); // remove "/"
      //cout << "Temp: " << temp << endl;
      wordvec path;
      path.push_back(words[0]);
      path.push_back(temp);
      if(checkIn(state, path) == true){
         fn_cd(state, path);
      }
      else{
         cout << "Error: Directory does not exist" << endl;
         return;
      }
   }

   wordvec spl = split(words[1], "/");
   string word = spl[spl.size() - 1];
   if(state.getCwd()->getContent()->getPtr(word) == nullptr){
      state.getCwd()->getContent()->mkdir(word);
      inode_ptr newDir = state.getCwd()->getContent()->getPtr(word);
      newDir->getContent()->getDirent()[".."] = state.getCwd();
   }
   else{
      cout << "Error: Directory Already Exists" << endl; 
   }
   state.setCwd(saveCurr);
}

void fn_prompt (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   state.prompt(words);
}

void fn_pwd (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   if(state.getCwd() == state.getRoot()){
      cout << "/" << endl;
      return;
   }
   inode_ptr save = state.getCwd();
   wordvec goParent;
   goParent.push_back(words[0]); 
   goParent.push_back("..");
   string name = "";
   while(state.getCwd() != state.getRoot()){
      name = "/" + state.getCwd()->getName() + name;
      fn_cd(state, goParent);
   }
   state.setCwd(save);
   cout << name << endl;
   return;
}

void fn_rm (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_rmr (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}
