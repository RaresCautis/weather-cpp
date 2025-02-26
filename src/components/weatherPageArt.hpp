#ifndef WEATHERPAGEART_H
#define WEATHERPAGEART_H
#include <weatherPage.hpp>
using namespace wPage;

namespace weatherPageArt {
const std::unordered_map<Status, std::string> statusToArt = {
    {Status::CLEAR, R"(
             
     \ | /
   '-.;;;.-'
  -==;;;;;==-   
   .-';;;'-.
     / | \
         
            
    )"},
    {Status::PARTLY_CLOUDY, R"(
             
      \   /
       .-.
   --- ( ) ---  
     .--~~--.
    (        )
    `--------'
             
    )"},
    {Status::CLOUDY, R"(
              
              
     .-~~-.  
  .-(      )-.
 (            ) 
  `----------'
              
              
    )"},
    {Status::OVERCAST, R"(
              
              
     .----.  
  .-(      )-.
 (            ) 
  `----------'
              
              
    )"},
    {Status::FOG, R"(
                
   ~~~~~~~~~~~  
    ~~~~~~~~~~~
   ~~~~~~~~~~~  
    ~~~~~~~~~~~
   ~~~~~~~~~~~  
    ~~~~~~~~~~~
                
    )"},
    {Status::DRIZZLE, R"(
               
      .--.    
   .-(    )-.   
   (         ) 
    `-------'  
       ' '     
      ' '    
           
    )"},
    {Status::LIGHT_RAIN, R"(
               
      .--.    
   .-(    )-.   
   (         ) 
    `-------'  
       ; ;    
      ; ;   
               
   )"},
    {Status::RAIN, R"(
               
      .--.    
   .-(    )-.   
   (         )  
    `-------'  
      ; : ;    
     ; : ;    
               
   )"},
    {Status::HEAVY_RAIN, R"(
               
      .--.     
   .-(    )-.   
   (         )  
    `-------'   
      ;:;:;:    
     ;:;:;:    
           
   )"},
    {Status::LIGHT_SNOW, R"(
           
      .--.     
   .-(    )-.   
   (         )  
    `-------'   
       * *     
      * *    
               
   )"},
    {Status::SNOW, R"(
               
      .--.   
   .-(    )-.   
   (         )  
    `-------'   
      * * *  
     * * *   
               
   )"},
    {Status::HEAVY_SNOW, R"(
               
      .--.   
   .-(    )-.   
   (         )  
    `-------'   
     *** ***  
    *** ***   
          
   )"},
    {Status::THUNDER, R"(
               
      .--.    
   .-(    )-.  
   (         )  
    `-------'  
       / /     
       / /     
               
    )"},
};
};

#endif
