#include<iostream>
#include<easyx.h>
#include<vector>//����˳���  
#include<ctime>

//��û��д���ƶ����߽紦��δ���
using namespace std;
/*С�����*/
class Sprite
{
public:Sprite():Sprite(0,0) {};
	  Sprite(int x, int y) :m_x(x), m_y(y), m_color(RED) {};
	  //��С���
	  virtual void draw() {//virtual Ϊ��ʵ�ֶ�̬�������Ķ���
		  //�����ɫ
		  setfillcolor(m_color);
		  //���ƾ���
		  fillrectangle(m_x, m_y, m_x + 10, m_y + 10);//���Ͻ����꣬���Ͻ�����
	  }
	  void draw2() {
		  setfillcolor(WHITE);
		  fillrectangle(m_x, m_y, m_x + 10, m_y + 10);
	  }
	  //��ͨ�ƶ�
	  void moveBy(int dx, int dy) {
		  m_x += dx;
		  m_y += dy;
	  }
	  //������ײ
	  bool collision(const Sprite& other) {
		  return m_x == other.m_x && m_y == other.m_y;
	  }
	  /*���߽�
	  bool boundary() {
		  if (m_x == 0 || m_x == 630 || m_y == 0 || m_y == 470) {
			  return true;
		  }
		  return false;
	  }*/
	  
protected://�̳еĶ����ǿ���ʹ�õ�
	int m_x;
	int m_y;
	COLORREF m_color;
};
/*����*/
class Snake:public Sprite {//�̳�Sprite
public:
	Snake():Snake(0,0){}
	Snake(int x, int y) :Sprite(x, y),dir(VK_RIGHT) {
		//��ʼ��������
		nodes.push_back(Sprite(300, 240));//��ͷ
		nodes.push_back(Sprite(310, 240));
		nodes.push_back(Sprite(320, 240));
	}
	void draw() override {//��д����
		nodes[0].draw2();
		for (int i = 1; i < nodes.size(); i++) {
			nodes[i].draw();
		}
	}
	/*�����ƶ�*/
	void bodyMove() {
		/*�Ӻ���ǰ����*/
		//���������ͷ�ƶ�
		for (size_t i = nodes.size() - 1; i > 0; i--) {
			nodes[i] = nodes[i-1];
		}
		//�ƶ���ͷ
		switch (dir) {
		case VK_UP:
			nodes[0].moveBy(0, -10);//��ͷ�����ƶ�
			break;
		case VK_DOWN:
			nodes[0].moveBy(0, 10);//��ͷ�����ƶ�
			break;
		case VK_LEFT:
			nodes[0].moveBy(-10, 0);//��ͷ�����ƶ�
			break;
		case VK_RIGHT:
			nodes[0].moveBy(10, 0);//��ͷ�����ƶ�
			break;
		}
		
	}
	//������˲�������ײ
	bool collision1(const Sprite &other) {
		return nodes[0].collision(other);
	}
	//�ߵĽ�������
	void increase() {
		nodes.push_back(Sprite());
	}
	/*�ı䷽��
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
	//���ɺܶ�����000
	std::vector<Sprite> nodes;//�ߵ����нڵ�
public:
	int dir;//��ʾ�ߵķ���
};
/*ʳ��*/
class Food :public Sprite {
public:
	Food():Sprite(0,0){
		changePos();
	}
	//�����ɫ
	void draw()override {
		setfillcolor(BLUE);
		solidellipse(m_x, m_y, m_x + 10, m_y + 10);
	}

	//�������ʳ�������,����Ӧ��Ϊ10��������
	void changePos() {
		m_x = rand() % 64 * 10;
		m_y = rand() % 48 * 10;
	}
	
};
/*��Ϸ����*/
class GameScene {
public:
	GameScene(){}
	void run() {
		BeginBatchDraw();//˫�����ͼ
		cleardevice();//����
		snake.draw();
		food.draw();//����ʳ��
		snakeEatFood();//�߳Ե�ʳ��
		EndBatchDraw();
		snake.bodyMove();//�������ƶ�


		
		//��ȡ��Ϣ
		ExMessage msg = { 0 };
		while (peekmessage(&msg, EM_KEY)) {//������Ϣʱ
			onMsg(msg);
		}

	}
	//����м�����Ϣ
	void onMsg(const ExMessage& msg) {
		/*�ı��ߵ����ͷ��򣬻�ȡ���̰���_getch()*/
		if (msg.message == WM_KEYDOWN) {
			//�жϾ�������ĸ����� ����virtual key code�������,���ߵķ���ֵ����
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
			
			//cout << msg.vkcode << endl;//�����Ϣ
		}
	}
	//����߳Ե���ʳ��
	void snakeEatFood() {
		if (snake.collision1(food)) {
			//�ߵĽ�������
			snake.increase();
			//ʳ�����
			food.changePos();
		}
	}
private:
	Snake snake;
	Food food;
};

int main() {
	initgraph(640, 480,EW_SHOWCONSOLE);
	//Sprite s(50, 50);//С���ʵ���
	//s.draw();
	//������������
	srand(time(NULL));
	
	GameScene scene;
	while (true) {
		
		scene.run();
		Sleep(100);
	}
	getchar();//��ֹ����
}