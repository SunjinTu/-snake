#include<iostream>
#include<easyx.h>
#include<vector>//容器顺序表  
#include<ctime>

//还没有写蛇移动到边界处如何处理
using namespace std;
/*小框框类*/
class Sprite
{
public:Sprite():Sprite(0,0) {};
	  Sprite(int x, int y) :m_x(x), m_y(y), m_color(RED) {};
	  //画小框框
	  virtual void draw() {//virtual 为了实现多态而产生的东西
		  //填充颜色
		  setfillcolor(m_color);
		  //绘制矩形
		  fillrectangle(m_x, m_y, m_x + 10, m_y + 10);//左上角坐标，右上角坐标
	  }
	  void draw2() {
		  setfillcolor(WHITE);
		  fillrectangle(m_x, m_y, m_x + 10, m_y + 10);
	  }
	  //普通移动
	  void moveBy(int dx, int dy) {
		  m_x += dx;
		  m_y += dy;
	  }
	  //设置碰撞
	  bool collision(const Sprite& other) {
		  return m_x == other.m_x && m_y == other.m_y;
	  }
	  /*到边界
	  bool boundary() {
		  if (m_x == 0 || m_x == 630 || m_y == 0 || m_y == 470) {
			  return true;
		  }
		  return false;
	  }*/
	  
protected://继承的儿子是可以使用的
	int m_x;
	int m_y;
	COLORREF m_color;
};
/*蛇类*/
class Snake:public Sprite {//继承Sprite
public:
	Snake():Snake(0,0){}
	Snake(int x, int y) :Sprite(x, y),dir(VK_RIGHT) {
		//初始化三节蛇
		nodes.push_back(Sprite(300, 240));//蛇头
		nodes.push_back(Sprite(310, 240));
		nodes.push_back(Sprite(320, 240));
	}
	void draw() override {//重写父类
		nodes[0].draw2();
		for (int i = 1; i < nodes.size(); i++) {
			nodes[i].draw();
		}
	}
	/*身体移动*/
	void bodyMove() {
		/*从后向前遍历*/
		//身体跟着蛇头移动
		for (size_t i = nodes.size() - 1; i > 0; i--) {
			nodes[i] = nodes[i-1];
		}
		//移动蛇头
		switch (dir) {
		case VK_UP:
			nodes[0].moveBy(0, -10);//蛇头向上移动
			break;
		case VK_DOWN:
			nodes[0].moveBy(0, 10);//蛇头向下移动
			break;
		case VK_LEFT:
			nodes[0].moveBy(-10, 0);//蛇头向左移动
			break;
		case VK_RIGHT:
			nodes[0].moveBy(10, 0);//蛇头向右移动
			break;
		}
		
	}
	//蛇与别人产生了碰撞
	bool collision1(const Sprite &other) {
		return nodes[0].collision(other);
	}
	//蛇的节数增加
	void increase() {
		nodes.push_back(Sprite());
	}
	/*改变方向
	void ChangeDirect() {
		switch (dir) {
			case VK_LEFT:
				dir = VK_RIGHT; break;
			case VK_RIGHT:
				dir = VK_LEFT; break;
			case VK_UP:
				dir = VK_DOWN; break;
			case VK_DOWN:
				dir = VK_UP; break;
		}
	}*/
private:
	//蛇由很多节组成000
	std::vector<Sprite> nodes;//蛇的所有节点
public:
	int dir;//表示蛇的方向
};
/*食物*/
class Food :public Sprite {
public:
	Food():Sprite(0,0){
		changePos();
	}
	//填充颜色
	void draw()override {
		setfillcolor(BLUE);
		solidellipse(m_x, m_y, m_x + 10, m_y + 10);
	}

	//随机生成食物的坐标,坐标应该为10的整数倍
	void changePos() {
		m_x = rand() % 64 * 10;
		m_y = rand() % 48 * 10;
	}
	
};
/*游戏场景*/
class GameScene {
public:
	GameScene(){}
	void run() {
		BeginBatchDraw();//双缓冲绘图
		cleardevice();//清屏
		snake.draw();
		food.draw();//画出食物
		snakeEatFood();//蛇吃到食物
		EndBatchDraw();
		snake.bodyMove();//蛇身体移动


		
		//获取消息
		ExMessage msg = { 0 };
		while (peekmessage(&msg, EM_KEY)) {//当有消息时
			onMsg(msg);
		}

	}
	//如果有键盘消息
	void onMsg(const ExMessage& msg) {
		/*改变蛇的移送方向，获取键盘案件_getch()*/
		if (msg.message == WM_KEYDOWN) {
			//判断具体的是哪个按键 按下virtual key code虚拟键码,将蛇的方向赋值好了
			switch (msg.vkcode) {
			case VK_UP:
				if(snake.dir!=VK_DOWN)
					snake.dir = msg.vkcode;
				break;
			case VK_DOWN:
				if (snake.dir != VK_UP)
					snake.dir = msg.vkcode;
				break;
			case VK_LEFT:
				if (snake.dir != VK_RIGHT)
					snake.dir = msg.vkcode;
				break;
			case VK_RIGHT:
				if (snake.dir != VK_LEFT)
					snake.dir = msg.vkcode;
				break;
			}
			
			//cout << msg.vkcode << endl;//输出消息
		}
	}
	//如果蛇吃到了食物
	void snakeEatFood() {
		if (snake.collision1(food)) {
			//蛇的节数增加
			snake.increase();
			//食物更新
			food.changePos();
		}
	}
private:
	Snake snake;
	Food food;
};

int main() {
	initgraph(640, 480,EW_SHOWCONSOLE);
	//Sprite s(50, 50);//小框框实验点
	//s.draw();
	//设计随机数种子
	srand(time(NULL));
	
	GameScene scene;
	while (true) {
		
		scene.run();
		Sleep(100);
	}
	getchar();//防止闪退
}