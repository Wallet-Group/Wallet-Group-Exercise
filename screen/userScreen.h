#pragma once
#include <memory>
#include <string>
#include "../models/User.h"
#include "../interface/UserService.h"

void showUserScreen(std::shared_ptr<User> user, UserService& userService); 