#include <stdio.h>
#include <chrono>
#include <thread>
#include <iostream>
#include <cmath>
#include <lebai/robot.hh>

void Thread1(lebai::l_master::Robot * robot){
  while(1)
  {
    robot->start_task("10017");
    robot->wait_move();
    std::cout<<"move done"<<std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
  }
  return;
}

void Thread2(lebai::l_master::Robot *robot) {
  while(1)
  {
    robot->get_target_joint_speed();
    robot->get_actual_joint_positions();
    robot->get_actual_tcp_pose();
    robot->get_actual_joint_positions();
    auto jp = robot->get_actual_joint_positions();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  return;
}

int main() {
  // 172.17.0.4
  lebai::l_master::Robot robot("192.168.1.100", true);
  // lebai::l_master::Robot robot("172.17.0.4", true);
  std::thread t1(Thread1, &robot);
  std::thread t2(Thread2, &robot);
  t1.join();
  t2.join();

}


// #include <pthread.h>
// #include <stdio.h>

// int Global;

// void *Thread1(void *x) {
//   Global++;
//   return NULL;
// }

// void *Thread2(void *x) {
//   Global--;
//   return NULL;
// }

// int main() {
//   pthread_t t[2];
//   pthread_create(&t[0], NULL, Thread1, NULL);
//   pthread_create(&t[1], NULL, Thread2, NULL);
//   pthread_join(t[0], NULL);
//   pthread_join(t[1], NULL);
// }