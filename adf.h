#pragma once
#ifndef ADF_H
#define ADF_H
#define MAX_LAYER 5
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace std;

vector<float> corners;
int adfcount;

typedef struct Cubecell {
	glm::vec3 positions[8];
	float value[8];
	struct Cubecell* son[8]; //八叉树结构
	bool contain;
	float stripe;
	int cubeindex;
}cubecell, * cubenode;

void push(int i, int j,cubenode head,int layer) {
	float highlight;
	if (layer == MAX_LAYER) {
		highlight = 2.0f;
	}
	else {
		highlight = 0.0f;
	}
	corners.push_back(head->positions[i].x);
	corners.push_back(head->positions[i].y);
	corners.push_back(head->positions[i].z);
	corners.push_back(highlight);
	corners.push_back(head->positions[j].x);
	corners.push_back(head->positions[j].y);
	corners.push_back(head->positions[j].z);
	corners.push_back(highlight);
}

void GenerateADF(cubenode head, int layer) {
	//cout << layer << endl;
	//传入头节点 递归的进行分割直到到达递归边界,即达到最大划分层数,或者不包含单位球，就返回
	if (!head->contain || layer == MAX_LAYER) {
		push(0, 1, head,layer);
		push(1, 2, head, layer);
		push(2, 3, head, layer);
		push(3, 0, head, layer);
		push(4, 5, head, layer);
		push(5, 6, head, layer);
		push(6, 7, head, layer);
		push(7, 4, head, layer);
		push(0, 4, head, layer);
		push(1, 5, head, layer);
		push(2, 6, head, layer);
		push(3, 7, head, layer);
		adfcount += 24;
		return;
	}

	//假设未到达最大划分层数，就继续划分，生成八个子体元
	for (int i = 0; i < 8; i++) {
		cubenode son = (cubenode)malloc(sizeof(cubecell));
		//对每个子体元进行坐标和值的计算
		son->stripe = head->stripe / 2;
		//计算每个子体元的初始点位置
		switch (i)
		{
		case 0:
			son->positions[0] = head->positions[0]; break;
		case 1:
			son->positions[0] = head->positions[0] + glm::vec3(0.f, 1.f, 0.f) * (son->stripe); break;
		case 2:
			son->positions[0] = head->positions[0] + glm::vec3(0.f, 1.f, 1.f) * (son->stripe); break;
		case 3:
			son->positions[0] = head->positions[0] + glm::vec3(0.f, 0.f, 1.f) * (son->stripe); break;
		case 4:
			son->positions[0] = head->positions[0] + glm::vec3(1.f, 0.f, 0.f) * (son->stripe); break;
		case 5:
			son->positions[0] = head->positions[0] + glm::vec3(1.f, 1.f, 0.f) * (son->stripe); break;
		case 6:
			son->positions[0] = head->positions[0] + glm::vec3(1.f, 1.f, 1.f) * (son->stripe); break;
		case 7:
			son->positions[0] = head->positions[0] + glm::vec3(1.f, 0.f, 1.f) * (son->stripe); break;
		default:
			break;
		}
		son->positions[1] = son->positions[0] + glm::vec3(0.f, 1.f, 0.f) * (son->stripe);
		son->positions[2] = son->positions[0] + glm::vec3(0.f, 1.f, 1.f) * (son->stripe);
		son->positions[3] = son->positions[0] + glm::vec3(0.f, 0.f, 1.f) * (son->stripe);
		son->positions[4] = son->positions[0] + glm::vec3(1.f, 0.f, 0.f) * (son->stripe);
		son->positions[5] = son->positions[0] + glm::vec3(1.f, 1.f, 0.f) * (son->stripe);
		son->positions[6] = son->positions[0] + glm::vec3(1.f, 1.f, 1.f) * (son->stripe);
		son->positions[7] = son->positions[0] + glm::vec3(1.f, 0.f, 1.f) * (son->stripe);
		//计算体素的值
		for (int i = 0; i < 8; i++) {
			son->value[i] = glm::distance(son->positions[i], glm::vec3(0, 0, 0));
		}
		//判断是否包含单位球
		son->cubeindex = 0;
		if (son->value[0] < 1.0f)son->cubeindex |= 1;
		if (son->value[1] < 1.0f)son->cubeindex |= 2;
		if (son->value[2] < 1.0f)son->cubeindex |= 4;
		if (son->value[3] < 1.0f)son->cubeindex |= 8;
		if (son->value[4] < 1.0f)son->cubeindex |= 16;
		if (son->value[5] < 1.0f)son->cubeindex |= 32;
		if (son->value[6] < 1.0f)son->cubeindex |= 64;
		if (son->value[7] < 1.0f)son->cubeindex |= 128;
		if (son->cubeindex != 0 && son->cubeindex != 255) son->contain = true;
		else son->contain = false;

		//绑定父节点
		head->son[i] = son;
	}

	//继续分割
	for (int i = 0; i < 8; i++) {
		GenerateADF(head->son[i], layer + 1);
	}
}

cubenode generateTree() {
	//初始化最初的节点
	cubenode head = (cubenode)malloc(sizeof(cubecell));
	head->positions[0] = glm::vec3(-1.f, -1.f, -1.f);
	head->positions[1] = glm::vec3(-1.f, 1.f, -1.f);
	head->positions[2] = glm::vec3(-1.f, 1.f, 1.f);
	head->positions[3] = glm::vec3(-1.f, -1.f, 1.f);
	head->positions[4] = glm::vec3(1.f, -1.f, -1.f);
	head->positions[5] = glm::vec3(1.f, 1.f, -1.f);
	head->positions[6] = glm::vec3(1.f, 1.f, 1.f);
	head->positions[7] = glm::vec3(1.f, -1.f, 1.f);
	head->value[0] = glm::distance(head->positions[0], glm::vec3(0, 0, 0));
	head->value[1] = glm::distance(head->positions[1], glm::vec3(0, 0, 0));
	head->value[2] = glm::distance(head->positions[2], glm::vec3(0, 0, 0));
	head->value[3] = glm::distance(head->positions[3], glm::vec3(0, 0, 0));
	head->value[4] = glm::distance(head->positions[4], glm::vec3(0, 0, 0));
	head->value[5] = glm::distance(head->positions[5], glm::vec3(0, 0, 0));
	head->value[6] = glm::distance(head->positions[6], glm::vec3(0, 0, 0));
	head->value[7] = glm::distance(head->positions[7], glm::vec3(0, 0, 0));
	head->contain = true;
	head->stripe = 2.f;
	head->cubeindex = 0;
	adfcount = 0;
	//开始划分
	GenerateADF(head, 0);
	return head;
}


#endif // !ADF_H

