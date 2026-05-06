#include "coroutine_cpp20/task.h"
#include "coroutine_cpp20/scheduler.h"
#include "business/login/login_handler.h"

using namespace business::login;

Task<void> async_login(LoginHandler* handler,
                       std::string req,
                       std::string* resp) {
    *resp = handler->Handle(req);
    co_return;
}

void HandleLoginCPP20(LoginHandler* handler,
                      const std::string& req,
                      std::string& resp) {
    auto t = async_login(handler, req, &resp);
    (void)t;
    Scheduler::Instance().Run();
}
