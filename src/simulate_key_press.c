#include "simulate_key_press.h"

// 模拟按键按下和释放
void simulate_key_press(WORD vk_code) {
	// 创建输入事件数组
	INPUT inputs[2] = { 0 }; //INPUT结构体是windows.h里的定义好的。inputs数组里每个元素就是1个输入事件。

	// 按下按键事件(锁定)
	inputs[0].type = INPUT_KEYBOARD; //设置第一个输入事件的类型为键盘输入。
	inputs[0].ki.wVk = vk_code;//设置第一个输入事件所要按下的虚拟键码。
	inputs[0].ki.dwFlags = 0;//标记这是一个按下按键的事件。默认为0，其实这行代码可以省略。

	// 释放按键事件(解锁)
	inputs[1].type = INPUT_KEYBOARD;//设置第二个输入事件的类型为键盘输入。
	inputs[1].ki.wVk = vk_code;//设置第二个输入事件所要按下的虚拟键码。
	inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;//标记这是一个释放按键的事件。必须标记，不然系统分辨不出这个按键信息是按下还是释放。

	// 发送输入事件。SendInput(事件结构体数量，事件结构体所在的数组，单个事件结构体的大小)。
	SendInput(2, inputs, sizeof(INPUT));//SendInput函数会将事件插入到系统输入流当中。
}


