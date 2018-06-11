#include"../../ResourceManager/Model/ModelManager.h"
#include<d3dx9.h>
#include"../Creator/ModelCreator.h"

void ModelManager::CreateSphere()
{

	float pos[] = {
		0.000000, 1.000000, 0.000000,
		0.074660, 0.980790, -0.180240,
		0.000000, 0.980790, -0.195090,
		0.000000, 1.000000, 0.000000,
		0.137950, 0.980790, -0.137950,
		0.000000, 1.000000, 0.000000,
		0.180240, 0.980790, -0.074660,
		0.000000, 1.000000, 0.000000,
		0.195090, 0.980790, 0.000000,
		0.000000, 1.000000, 0.000000,
		0.180240, 0.980790, 0.074660,
		0.000000, 1.000000, 0.000000,
		0.137950, 0.980790, 0.137950,
		0.000000, 1.000000, 0.000000,
		0.074660, 0.980790, 0.180240,
		0.000000, 1.000000, 0.000000,
		0.000000, 0.980790, 0.195090,
		0.000000, 1.000000, 0.000000,
		-0.074660, 0.980790, 0.180240,
		0.000000, 1.000000, 0.000000,
		-0.137950, 0.980790, 0.137950,
		0.000000, 1.000000, 0.000000,
		-0.180240, 0.980790, 0.074660,
		0.000000, 1.000000, 0.000000,
		-0.195090, 0.980790, -0.000000,
		0.000000, 1.000000, 0.000000,
		-0.180240, 0.980790, -0.074660,
		0.000000, 1.000000, 0.000000,
		-0.137950, 0.980790, -0.137950,
		0.000000, 1.000000, 0.000000,
		-0.074660, 0.980790, -0.180240,
		0.000000, 1.000000, 0.000000,
		0.000000, 0.980790, -0.195090,
		0.146450, 0.923880, -0.353550,
		0.000000, 0.923880, -0.382680,
		0.270600, 0.923880, -0.270600,
		0.353550, 0.923880, -0.146450,
		0.382680, 0.923880, 0.000000,
		0.353550, 0.923880, 0.146450,
		0.270600, 0.923880, 0.270600,
		0.146450, 0.923880, 0.353550,
		0.000000, 0.923880, 0.382680,
		-0.146450, 0.923880, 0.353550,
		-0.270600, 0.923880, 0.270600,
		-0.353550, 0.923880, 0.146450,
		-0.382680, 0.923880, -0.000000,
		-0.353550, 0.923880, -0.146450,
		-0.270600, 0.923880, -0.270600,
		-0.146450, 0.923880, -0.353550,
		0.000000, 0.923880, -0.382680,
		0.212610, 0.831470, -0.513280,
		0.000000, 0.831470, -0.555570,
		0.392850, 0.831470, -0.392850,
		0.513280, 0.831470, -0.212610,
		0.555570, 0.831470, 0.000000,
		0.513280, 0.831470, 0.212610,
		0.392850, 0.831470, 0.392850,
		0.212610, 0.831470, 0.513280,
		0.000000, 0.831470, 0.555570,
		-0.212610, 0.831470, 0.513280,
		-0.392850, 0.831470, 0.392850,
		-0.513280, 0.831470, 0.212610,
		-0.555570, 0.831470, -0.000000,
		-0.513280, 0.831470, -0.212610,
		-0.392850, 0.831470, -0.392850,
		-0.212610, 0.831470, -0.513280,
		0.000000, 0.831470, -0.555570,
		0.270600, 0.707110, -0.653280,
		0.000000, 0.707110, -0.707110,
		0.500000, 0.707110, -0.500000,
		0.653280, 0.707110, -0.270600,
		0.707110, 0.707110, 0.000000,
		0.653280, 0.707110, 0.270600,
		0.500000, 0.707110, 0.500000,
		0.270600, 0.707110, 0.653280,
		0.000000, 0.707110, 0.707110,
		-0.270600, 0.707110, 0.653280,
		-0.500000, 0.707110, 0.500000,
		-0.653280, 0.707110, 0.270600,
		-0.707110, 0.707110, -0.000000,
		-0.653280, 0.707110, -0.270600,
		-0.500000, 0.707110, -0.500000,
		-0.270600, 0.707110, -0.653280,
		0.000000, 0.707110, -0.707110,
		0.318190, 0.555570, -0.768180,
		0.000000, 0.555570, -0.831470,
		0.587940, 0.555570, -0.587940,
		0.768180, 0.555570, -0.318190,
		0.831470, 0.555570, 0.000000,
		0.768180, 0.555570, 0.318190,
		0.587940, 0.555570, 0.587940,
		0.318190, 0.555570, 0.768180,
		0.000000, 0.555570, 0.831470,
		-0.318190, 0.555570, 0.768180,
		-0.587940, 0.555570, 0.587940,
		-0.768180, 0.555570, 0.318190,
		-0.831470, 0.555570, -0.000000,
		-0.768180, 0.555570, -0.318190,
		-0.587940, 0.555570, -0.587940,
		-0.318190, 0.555570, -0.768180,
		0.000000, 0.555570, -0.831470,
		0.353550, 0.382680, -0.853550,
		0.000000, 0.382680, -0.923880,
		0.653280, 0.382680, -0.653280,
		0.853550, 0.382680, -0.353550,
		0.923880, 0.382680, 0.000000,
		0.853550, 0.382680, 0.353550,
		0.653280, 0.382680, 0.653280,
		0.353550, 0.382680, 0.853550,
		0.000000, 0.382680, 0.923880,
		-0.353550, 0.382680, 0.853550,
		-0.653280, 0.382680, 0.653280,
		-0.853550, 0.382680, 0.353550,
		-0.923880, 0.382680, -0.000000,
		-0.853550, 0.382680, -0.353550,
		-0.653280, 0.382680, -0.653280,
		-0.353550, 0.382680, -0.853550,
		0.000000, 0.382680, -0.923880,
		0.375330, 0.195090, -0.906130,
		0.000000, 0.195090, -0.980790,
		0.693520, 0.195090, -0.693520,
		0.906130, 0.195090, -0.375330,
		0.980790, 0.195090, 0.000000,
		0.906130, 0.195090, 0.375330,
		0.693520, 0.195090, 0.693520,
		0.375330, 0.195090, 0.906130,
		0.000000, 0.195090, 0.980790,
		-0.375330, 0.195090, 0.906130,
		-0.693520, 0.195090, 0.693520,
		-0.906130, 0.195090, 0.375330,
		-0.980790, 0.195090, -0.000000,
		-0.906130, 0.195090, -0.375330,
		-0.693520, 0.195090, -0.693520,
		-0.375330, 0.195090, -0.906130,
		0.000000, 0.195090, -0.980790,
		0.382680, 0.000000, -0.923880,
		0.000000, 0.000000, -1.000000,
		0.707110, 0.000000, -0.707110,
		0.923880, 0.000000, -0.382680,
		1.000000, 0.000000, 0.000000,
		0.923880, 0.000000, 0.382680,
		0.707110, 0.000000, 0.707110,
		0.382680, 0.000000, 0.923880,
		0.000000, 0.000000, 1.000000,
		-0.382680, 0.000000, 0.923880,
		-0.707110, 0.000000, 0.707110,
		-0.923880, 0.000000, 0.382680,
		-1.000000, 0.000000, -0.000000,
		-0.923880, 0.000000, -0.382680,
		-0.707110, 0.000000, -0.707110,
		-0.382680, 0.000000, -0.923880,
		0.000000, 0.000000, -1.000000,
		0.375330, -0.195090, -0.906130,
		0.000000, -0.195090, -0.980790,
		0.693520, -0.195090, -0.693520,
		0.906130, -0.195090, -0.375330,
		0.980790, -0.195090, 0.000000,
		0.906130, -0.195090, 0.375330,
		0.693520, -0.195090, 0.693520,
		0.375330, -0.195090, 0.906130,
		0.000000, -0.195090, 0.980790,
		-0.375330, -0.195090, 0.906130,
		-0.693520, -0.195090, 0.693520,
		-0.906130, -0.195090, 0.375330,
		-0.980790, -0.195090, -0.000000,
		-0.906130, -0.195090, -0.375330,
		-0.693520, -0.195090, -0.693520,
		-0.375330, -0.195090, -0.906130,
		0.000000, -0.195090, -0.980790,
		0.353550, -0.382680, -0.853550,
		0.000000, -0.382680, -0.923880,
		0.653280, -0.382680, -0.653280,
		0.853550, -0.382680, -0.353550,
		0.923880, -0.382680, 0.000000,
		0.853550, -0.382680, 0.353550,
		0.653280, -0.382680, 0.653280,
		0.353550, -0.382680, 0.853550,
		0.000000, -0.382680, 0.923880,
		-0.353550, -0.382680, 0.853550,
		-0.653280, -0.382680, 0.653280,
		-0.853550, -0.382680, 0.353550,
		-0.923880, -0.382680, -0.000000,
		-0.853550, -0.382680, -0.353550,
		-0.653280, -0.382680, -0.653280,
		-0.353550, -0.382680, -0.853550,
		0.000000, -0.382680, -0.923880,
		0.318190, -0.555570, -0.768180,
		0.000000, -0.555570, -0.831470,
		0.587940, -0.555570, -0.587940,
		0.768180, -0.555570, -0.318190,
		0.831470, -0.555570, 0.000000,
		0.768180, -0.555570, 0.318190,
		0.587940, -0.555570, 0.587940,
		0.318190, -0.555570, 0.768180,
		0.000000, -0.555570, 0.831470,
		-0.318190, -0.555570, 0.768180,
		-0.587940, -0.555570, 0.587940,
		-0.768180, -0.555570, 0.318190,
		-0.831470, -0.555570, -0.000000,
		-0.768180, -0.555570, -0.318190,
		-0.587940, -0.555570, -0.587940,
		-0.318190, -0.555570, -0.768180,
		0.000000, -0.555570, -0.831470,
		0.270600, -0.707110, -0.653280,
		0.000000, -0.707110, -0.707110,
		0.500000, -0.707110, -0.500000,
		0.653280, -0.707110, -0.270600,
		0.707110, -0.707110, 0.000000,
		0.653280, -0.707110, 0.270600,
		0.500000, -0.707110, 0.500000,
		0.270600, -0.707110, 0.653280,
		0.000000, -0.707110, 0.707110,
		-0.270600, -0.707110, 0.653280,
		-0.500000, -0.707110, 0.500000,
		-0.653280, -0.707110, 0.270600,
		-0.707110, -0.707110, -0.000000,
		-0.653280, -0.707110, -0.270600,
		-0.500000, -0.707110, -0.500000,
		-0.270600, -0.707110, -0.653280,
		0.000000, -0.707110, -0.707110,
		0.212610, -0.831470, -0.513280,
		0.000000, -0.831470, -0.555570,
		0.392850, -0.831470, -0.392850,
		0.513280, -0.831470, -0.212610,
		0.555570, -0.831470, 0.000000,
		0.513280, -0.831470, 0.212610,
		0.392850, -0.831470, 0.392850,
		0.212610, -0.831470, 0.513280,
		0.000000, -0.831470, 0.555570,
		-0.212610, -0.831470, 0.513280,
		-0.392850, -0.831470, 0.392850,
		-0.513280, -0.831470, 0.212610,
		-0.555570, -0.831470, -0.000000,
		-0.513280, -0.831470, -0.212610,
		-0.392850, -0.831470, -0.392850,
		-0.212610, -0.831470, -0.513280,
		0.000000, -0.831470, -0.555570,
		0.146450, -0.923880, -0.353550,
		0.000000, -0.923880, -0.382680,
		0.270600, -0.923880, -0.270600,
		0.353550, -0.923880, -0.146450,
		0.382680, -0.923880, 0.000000,
		0.353550, -0.923880, 0.146450,
		0.270600, -0.923880, 0.270600,
		0.146450, -0.923880, 0.353550,
		0.000000, -0.923880, 0.382680,
		-0.146450, -0.923880, 0.353550,
		-0.270600, -0.923880, 0.270600,
		-0.353550, -0.923880, 0.146450,
		-0.382680, -0.923880, -0.000000,
		-0.353550, -0.923880, -0.146450,
		-0.270600, -0.923880, -0.270600,
		-0.146450, -0.923880, -0.353550,
		0.000000, -0.923880, -0.382680,
		0.074660, -0.980790, -0.180240,
		0.000000, -0.980790, -0.195090,
		0.137950, -0.980790, -0.137950,
		0.180240, -0.980790, -0.074660,
		0.195090, -0.980790, 0.000000,
		0.180240, -0.980790, 0.074660,
		0.137950, -0.980790, 0.137950,
		0.074660, -0.980790, 0.180240,
		0.000000, -0.980790, 0.195090,
		-0.074660, -0.980790, 0.180240,
		-0.137950, -0.980790, 0.137950,
		-0.180240, -0.980790, 0.074660,
		-0.195090, -0.980790, -0.000000,
		-0.180240, -0.980790, -0.074660,
		-0.137950, -0.980790, -0.137950,
		-0.074660, -0.980790, -0.180240,
		0.000000, -0.980790, -0.195090,
		0.000000, -1.000000, -0.000000,
		0.000000, -1.000000, -0.000000,
		0.000000, -1.000000, -0.000000,
		0.000000, -1.000000, -0.000000,
		0.000000, -1.000000, -0.000000,
		0.000000, -1.000000, -0.000000,
		0.000000, -1.000000, -0.000000,
		0.000000, -1.000000, -0.000000,
		0.000000, -1.000000, -0.000000,
		0.000000, -1.000000, -0.000000,
		0.000000, -1.000000, -0.000000,
		0.000000, -1.000000, -0.000000,
		0.000000, -1.000000, -0.000000,
		0.000000, -1.000000, -0.000000,
		0.000000, -1.000000, -0.000000,
		0.000000, -1.000000, -0.000000,
	};
	float normal[] = {
		0.000000, 1.000000, 0.000000,
		0.074631, 0.980799, -0.180174,
		0.000000, 0.980799, -0.195019,
		0.000000, 1.000000, 0.000000,
		0.137899, 0.980800, -0.137899,
		0.000000, 1.000000, 0.000000,
		0.180174, 0.980799, -0.074631,
		0.000000, 1.000000, 0.000000,
		0.195019, 0.980799, 0.000000,
		0.000000, 1.000000, 0.000000,
		0.180174, 0.980799, 0.074631,
		0.000000, 1.000000, 0.000000,
		0.137899, 0.980800, 0.137899,
		0.000000, 1.000000, 0.000000,
		0.074631, 0.980799, 0.180174,
		0.000000, 1.000000, 0.000000,
		0.000000, 0.980799, 0.195019,
		0.000000, 1.000000, 0.000000,
		-0.074631, 0.980799, 0.180174,
		0.000000, 1.000000, 0.000000,
		-0.137899, 0.980800, 0.137899,
		0.000000, 1.000000, 0.000000,
		-0.180174, 0.980799, 0.074631,
		0.000000, 1.000000, 0.000000,
		-0.195019, 0.980799, 0.000000,
		0.000000, 1.000000, 0.000000,
		-0.180174, 0.980799, -0.074631,
		0.000000, 1.000000, 0.000000,
		-0.137899, 0.980799, -0.137900,
		0.000000, 1.000000, 0.000000,
		-0.074631, 0.980799, -0.180174,
		0.000000, 1.000000, 0.000000,
		0.000000, 0.980799, -0.195019,
		0.146399, 0.923931, -0.353439,
		0.000000, 0.923931, -0.382560,
		0.270511, 0.923931, -0.270511,
		0.353439, 0.923931, -0.146399,
		0.382560, 0.923931, 0.000000,
		0.353439, 0.923931, 0.146399,
		0.270511, 0.923931, 0.270511,
		0.146399, 0.923931, 0.353439,
		0.000000, 0.923931, 0.382560,
		-0.146399, 0.923931, 0.353439,
		-0.270511, 0.923931, 0.270511,
		-0.353439, 0.923931, 0.146399,
		-0.382560, 0.923931, 0.000000,
		-0.353439, 0.923931, -0.146399,
		-0.270511, 0.923931, -0.270511,
		-0.146399, 0.923931, -0.353439,
		0.000000, 0.923931, -0.382560,
		0.212552, 0.831566, -0.513147,
		0.000000, 0.831566, -0.555426,
		0.392745, 0.831566, -0.392745,
		0.513147, 0.831566, -0.212552,
		0.555426, 0.831566, 0.000000,
		0.513147, 0.831566, 0.212552,
		0.392745, 0.831566, 0.392745,
		0.212552, 0.831566, 0.513147,
		0.000000, 0.831566, 0.555426,
		-0.212552, 0.831566, 0.513147,
		-0.392745, 0.831566, 0.392746,
		-0.513147, 0.831566, 0.212552,
		-0.555426, 0.831566, 0.000000,
		-0.513147, 0.831566, -0.212552,
		-0.392745, 0.831566, -0.392746,
		-0.212552, 0.831566, -0.513147,
		0.000000, 0.831566, -0.555426,
		0.270548, 0.707239, -0.653160,
		0.000000, 0.707239, -0.706975,
		0.499907, 0.707239, -0.499907,
		0.653160, 0.707239, -0.270548,
		0.706975, 0.707239, 0.000000,
		0.653160, 0.707239, 0.270548,
		0.499907, 0.707239, 0.499907,
		0.270548, 0.707239, 0.653160,
		0.000000, 0.707239, 0.706975,
		-0.270547, 0.707239, 0.653160,
		-0.499907, 0.707239, 0.499907,
		-0.653160, 0.707238, 0.270548,
		-0.706975, 0.707239, 0.000000,
		-0.653160, 0.707239, -0.270548,
		-0.499907, 0.707239, -0.499907,
		-0.270547, 0.707239, -0.653160,
		0.000000, 0.707239, -0.706975,
		0.318153, 0.555712, -0.768090,
		0.000000, 0.555712, -0.831375,
		0.587871, 0.555712, -0.587871,
		0.768090, 0.555712, -0.318153,
		0.831375, 0.555712, 0.000000,
		0.768090, 0.555712, 0.318153,
		0.587871, 0.555712, 0.587871,
		0.318153, 0.555712, 0.768090,
		0.000000, 0.555712, 0.831375,
		-0.318153, 0.555712, 0.768090,
		-0.587870, 0.555712, 0.587871,
		-0.768090, 0.555712, 0.318153,
		-0.831375, 0.555712, 0.000000,
		-0.768090, 0.555712, -0.318153,
		-0.587870, 0.555712, -0.587871,
		-0.318153, 0.555712, -0.768090,
		0.000000, 0.555712, -0.831375,
		0.353534, 0.382803, -0.853508,
		0.000000, 0.382803, -0.923830,
		0.653246, 0.382803, -0.653246,
		0.853508, 0.382803, -0.353534,
		0.923830, 0.382803, 0.000000,
		0.853508, 0.382803, 0.353534,
		0.653246, 0.382803, 0.653246,
		0.353534, 0.382803, 0.853508,
		0.000000, 0.382803, 0.923830,
		-0.353534, 0.382803, 0.853508,
		-0.653246, 0.382804, 0.653246,
		-0.853508, 0.382803, 0.353534,
		-0.923830, 0.382803, 0.000000,
		-0.853508, 0.382803, -0.353534,
		-0.653246, 0.382803, -0.653247,
		-0.353534, 0.382803, -0.853508,
		0.000000, 0.382803, -0.923830,
		0.375325, 0.195159, -0.906115,
		0.000000, 0.195159, -0.980772,
		0.693510, 0.195159, -0.693510,
		0.906115, 0.195159, -0.375325,
		0.980772, 0.195159, 0.000000,
		0.906115, 0.195159, 0.375325,
		0.693510, 0.195159, 0.693510,
		0.375325, 0.195159, 0.906115,
		0.000000, 0.195159, 0.980772,
		-0.375325, 0.195159, 0.906115,
		-0.693510, 0.195159, 0.693510,
		-0.906115, 0.195159, 0.375325,
		-0.980772, 0.195159, 0.000000,
		-0.906115, 0.195159, -0.375325,
		-0.693510, 0.195159, -0.693510,
		-0.375325, 0.195159, -0.906115,
		0.000000, 0.195159, -0.980772,
		0.382683, 0.000000, -0.923880,
		0.000000, 0.000000, -1.000000,
		0.707107, 0.000000, -0.707107,
		0.923880, 0.000000, -0.382683,
		1.000000, 0.000000, 0.000000,
		0.923879, 0.000000, 0.382684,
		0.707107, 0.000000, 0.707107,
		0.382683, 0.000000, 0.923880,
		0.000000, 0.000000, 1.000000,
		-0.382683, 0.000000, 0.923880,
		-0.707107, 0.000000, 0.707107,
		-0.923879, 0.000000, 0.382684,
		-1.000000, 0.000000, 0.000000,
		-0.923879, 0.000000, -0.382684,
		-0.707107, 0.000000, -0.707107,
		-0.382683, 0.000000, -0.923880,
		0.000000, 0.000000, -1.000000,
		0.375325, -0.195159, -0.906115,
		0.000000, -0.195159, -0.980772,
		0.693510, -0.195159, -0.693510,
		0.906115, -0.195159, -0.375325,
		0.980772, -0.195159, 0.000000,
		0.906115, -0.195159, 0.375325,
		0.693510, -0.195159, 0.693510,
		0.375325, -0.195159, 0.906115,
		0.000000, -0.195159, 0.980772,
		-0.375325, -0.195159, 0.906115,
		-0.693510, -0.195159, 0.693510,
		-0.906115, -0.195159, 0.375325,
		-0.980772, -0.195159, 0.000000,
		-0.906115, -0.195159, -0.375325,
		-0.693510, -0.195159, -0.693511,
		-0.375325, -0.195159, -0.906115,
		0.000000, -0.195159, -0.980772,
		0.353534, -0.382803, -0.853508,
		0.000000, -0.382803, -0.923830,
		0.653246, -0.382803, -0.653246,
		0.853508, -0.382803, -0.353534,
		0.923830, -0.382803, 0.000000,
		0.853508, -0.382803, 0.353534,
		0.653246, -0.382803, 0.653246,
		0.353534, -0.382803, 0.853508,
		0.000000, -0.382803, 0.923830,
		-0.353534, -0.382803, 0.853508,
		-0.653246, -0.382803, 0.653247,
		-0.853508, -0.382803, 0.353534,
		-0.923830, -0.382803, 0.000000,
		-0.853508, -0.382803, -0.353534,
		-0.653246, -0.382803, -0.653247,
		-0.353534, -0.382803, -0.853508,
		0.000000, -0.382803, -0.923830,
		0.318153, -0.555712, -0.768090,
		0.000000, -0.555712, -0.831375,
		0.587871, -0.555712, -0.587871,
		0.768090, -0.555712, -0.318153,
		0.831375, -0.555712, 0.000000,
		0.768090, -0.555712, 0.318153,
		0.587871, -0.555712, 0.587871,
		0.318153, -0.555712, 0.768090,
		0.000000, -0.555712, 0.831375,
		-0.318153, -0.555712, 0.768090,
		-0.587870, -0.555712, 0.587871,
		-0.768090, -0.555712, 0.318153,
		-0.831375, -0.555712, 0.000000,
		-0.768090, -0.555712, -0.318153,
		-0.587870, -0.555712, -0.587871,
		-0.318153, -0.555712, -0.768090,
		0.000000, -0.555712, -0.831375,
		0.270548, -0.707239, -0.653160,
		0.000000, -0.707238, -0.706975,
		0.499907, -0.707239, -0.499907,
		0.653160, -0.707239, -0.270548,
		0.706975, -0.707239, 0.000000,
		0.653160, -0.707239, 0.270548,
		0.499907, -0.707239, 0.499907,
		0.270548, -0.707239, 0.653160,
		0.000000, -0.707238, 0.706975,
		-0.270547, -0.707239, 0.653160,
		-0.499907, -0.707239, 0.499907,
		-0.653160, -0.707239, 0.270548,
		-0.706975, -0.707238, 0.000000,
		-0.653160, -0.707239, -0.270548,
		-0.499907, -0.707239, -0.499907,
		-0.270547, -0.707239, -0.653160,
		0.000000, -0.707238, -0.706975,
		0.212552, -0.831566, -0.513147,
		0.000000, -0.831566, -0.555426,
		0.392746, -0.831566, -0.392745,
		0.513147, -0.831566, -0.212552,
		0.555426, -0.831566, 0.000000,
		0.513147, -0.831566, 0.212552,
		0.392745, -0.831566, 0.392745,
		0.212552, -0.831566, 0.513147,
		0.000000, -0.831566, 0.555426,
		-0.212552, -0.831566, 0.513147,
		-0.392745, -0.831566, 0.392746,
		-0.513147, -0.831566, 0.212552,
		-0.555426, -0.831566, 0.000000,
		-0.513147, -0.831566, -0.212552,
		-0.392745, -0.831566, -0.392746,
		-0.212552, -0.831566, -0.513147,
		0.000000, -0.831566, -0.555426,
		0.146399, -0.923931, -0.353439,
		0.000000, -0.923931, -0.382560,
		0.270511, -0.923931, -0.270511,
		0.353439, -0.923931, -0.146399,
		0.382560, -0.923931, 0.000000,
		0.353439, -0.923931, 0.146399,
		0.270511, -0.923931, 0.270511,
		0.146399, -0.923931, 0.353439,
		0.000000, -0.923931, 0.382560,
		-0.146399, -0.923931, 0.353439,
		-0.270511, -0.923931, 0.270511,
		-0.353439, -0.923931, 0.146399,
		-0.382560, -0.923931, 0.000000,
		-0.353439, -0.923931, -0.146399,
		-0.270510, -0.923931, -0.270511,
		-0.146399, -0.923931, -0.353439,
		0.000000, -0.923931, -0.382560,
		0.074630, -0.980800, -0.180174,
		0.000000, -0.980799, -0.195019,
		0.137899, -0.980800, -0.137899,
		0.180174, -0.980800, -0.074630,
		0.195019, -0.980799, 0.000000,
		0.180174, -0.980799, 0.074631,
		0.137899, -0.980800, 0.137899,
		0.074631, -0.980799, 0.180174,
		0.000000, -0.980799, 0.195019,
		-0.074630, -0.980800, 0.180174,
		-0.137899, -0.980800, 0.137899,
		-0.180174, -0.980799, 0.074631,
		-0.195019, -0.980799, 0.000000,
		-0.180174, -0.980799, -0.074631,
		-0.137899, -0.980800, -0.137899,
		-0.074630, -0.980800, -0.180174,
		0.000000, -0.980799, -0.195019,
		0.000000, -1.000000, 0.000000,
		0.000000, -1.000000, 0.000000,
		0.000000, -1.000000, 0.000000,
		0.000000, -1.000000, 0.000000,
		0.000000, -1.000000, 0.000000,
		0.000000, -1.000000, 0.000000,
		0.000000, -1.000000, 0.000000,
		0.000000, -1.000000, 0.000000,
		0.000000, -1.000000, 0.000000,
		0.000000, -1.000000, 0.000000,
		0.000000, -1.000000, 0.000000,
		0.000000, -1.000000, 0.000000,
		0.000000, -1.000000, 0.000000,
		0.000000, -1.000000, 0.000000,
		0.000000, -1.000000, 0.000000,
		0.000000, -1.000000, 0.000000,
	};
	float uv[] = {
		0.031250, 0.000000,
		0.062500, 0.062500,
		0.000000, 0.062500,
		0.093750, 0.000000,
		0.125000, 0.062500,
		0.156250, 0.000000,
		0.187500, 0.062500,
		0.218750, 0.000000,
		0.250000, 0.062500,
		0.281250, 0.000000,
		0.312500, 0.062500,
		0.343750, 0.000000,
		0.375000, 0.062500,
		0.406250, 0.000000,
		0.437500, 0.062500,
		0.468750, 0.000000,
		0.500000, 0.062500,
		0.531250, 0.000000,
		0.562500, 0.062500,
		0.593750, 0.000000,
		0.625000, 0.062500,
		0.656250, 0.000000,
		0.687500, 0.062500,
		0.718750, 0.000000,
		0.750000, 0.062500,
		0.781250, 0.000000,
		0.812500, 0.062500,
		0.843750, 0.000000,
		0.875000, 0.062500,
		0.906250, 0.000000,
		0.937500, 0.062500,
		0.968750, 0.000000,
		1.000000, 0.062500,
		0.062500, 0.125000,
		0.000000, 0.125000,
		0.125000, 0.125000,
		0.187500, 0.125000,
		0.250000, 0.125000,
		0.312500, 0.125000,
		0.375000, 0.125000,
		0.437500, 0.125000,
		0.500000, 0.125000,
		0.562500, 0.125000,
		0.625000, 0.125000,
		0.687500, 0.125000,
		0.750000, 0.125000,
		0.812500, 0.125000,
		0.875000, 0.125000,
		0.937500, 0.125000,
		1.000000, 0.125000,
		0.062500, 0.187500,
		0.000000, 0.187500,
		0.125000, 0.187500,
		0.187500, 0.187500,
		0.250000, 0.187500,
		0.312500, 0.187500,
		0.375000, 0.187500,
		0.437500, 0.187500,
		0.500000, 0.187500,
		0.562500, 0.187500,
		0.625000, 0.187500,
		0.687500, 0.187500,
		0.750000, 0.187500,
		0.812500, 0.187500,
		0.875000, 0.187500,
		0.937500, 0.187500,
		1.000000, 0.187500,
		0.062500, 0.250000,
		0.000000, 0.250000,
		0.125000, 0.250000,
		0.187500, 0.250000,
		0.250000, 0.250000,
		0.312500, 0.250000,
		0.375000, 0.250000,
		0.437500, 0.250000,
		0.500000, 0.250000,
		0.562500, 0.250000,
		0.625000, 0.250000,
		0.687500, 0.250000,
		0.750000, 0.250000,
		0.812500, 0.250000,
		0.875000, 0.250000,
		0.937500, 0.250000,
		1.000000, 0.250000,
		0.062500, 0.312500,
		0.000000, 0.312500,
		0.125000, 0.312500,
		0.187500, 0.312500,
		0.250000, 0.312500,
		0.312500, 0.312500,
		0.375000, 0.312500,
		0.437500, 0.312500,
		0.500000, 0.312500,
		0.562500, 0.312500,
		0.625000, 0.312500,
		0.687500, 0.312500,
		0.750000, 0.312500,
		0.812500, 0.312500,
		0.875000, 0.312500,
		0.937500, 0.312500,
		1.000000, 0.312500,
		0.062500, 0.375000,
		0.000000, 0.375000,
		0.125000, 0.375000,
		0.187500, 0.375000,
		0.250000, 0.375000,
		0.312500, 0.375000,
		0.375000, 0.375000,
		0.437500, 0.375000,
		0.500000, 0.375000,
		0.562500, 0.375000,
		0.625000, 0.375000,
		0.687500, 0.375000,
		0.750000, 0.375000,
		0.812500, 0.375000,
		0.875000, 0.375000,
		0.937500, 0.375000,
		1.000000, 0.375000,
		0.062500, 0.437500,
		0.000000, 0.437500,
		0.125000, 0.437500,
		0.187500, 0.437500,
		0.250000, 0.437500,
		0.312500, 0.437500,
		0.375000, 0.437500,
		0.437500, 0.437500,
		0.500000, 0.437500,
		0.562500, 0.437500,
		0.625000, 0.437500,
		0.687500, 0.437500,
		0.750000, 0.437500,
		0.812500, 0.437500,
		0.875000, 0.437500,
		0.937500, 0.437500,
		1.000000, 0.437500,
		0.062500, 0.500000,
		0.000000, 0.500000,
		0.125000, 0.500000,
		0.187500, 0.500000,
		0.250000, 0.500000,
		0.312500, 0.500000,
		0.375000, 0.500000,
		0.437500, 0.500000,
		0.500000, 0.500000,
		0.562500, 0.500000,
		0.625000, 0.500000,
		0.687500, 0.500000,
		0.750000, 0.500000,
		0.812500, 0.500000,
		0.875000, 0.500000,
		0.937500, 0.500000,
		1.000000, 0.500000,
		0.062500, 0.562500,
		0.000000, 0.562500,
		0.125000, 0.562500,
		0.187500, 0.562500,
		0.250000, 0.562500,
		0.312500, 0.562500,
		0.375000, 0.562500,
		0.437500, 0.562500,
		0.500000, 0.562500,
		0.562500, 0.562500,
		0.625000, 0.562500,
		0.687500, 0.562500,
		0.750000, 0.562500,
		0.812500, 0.562500,
		0.875000, 0.562500,
		0.937500, 0.562500,
		1.000000, 0.562500,
		0.062500, 0.625000,
		0.000000, 0.625000,
		0.125000, 0.625000,
		0.187500, 0.625000,
		0.250000, 0.625000,
		0.312500, 0.625000,
		0.375000, 0.625000,
		0.437500, 0.625000,
		0.500000, 0.625000,
		0.562500, 0.625000,
		0.625000, 0.625000,
		0.687500, 0.625000,
		0.750000, 0.625000,
		0.812500, 0.625000,
		0.875000, 0.625000,
		0.937500, 0.625000,
		1.000000, 0.625000,
		0.062500, 0.687500,
		0.000000, 0.687500,
		0.125000, 0.687500,
		0.187500, 0.687500,
		0.250000, 0.687500,
		0.312500, 0.687500,
		0.375000, 0.687500,
		0.437500, 0.687500,
		0.500000, 0.687500,
		0.562500, 0.687500,
		0.625000, 0.687500,
		0.687500, 0.687500,
		0.750000, 0.687500,
		0.812500, 0.687500,
		0.875000, 0.687500,
		0.937500, 0.687500,
		1.000000, 0.687500,
		0.062500, 0.750000,
		0.000000, 0.750000,
		0.125000, 0.750000,
		0.187500, 0.750000,
		0.250000, 0.750000,
		0.312500, 0.750000,
		0.375000, 0.750000,
		0.437500, 0.750000,
		0.500000, 0.750000,
		0.562500, 0.750000,
		0.625000, 0.750000,
		0.687500, 0.750000,
		0.750000, 0.750000,
		0.812500, 0.750000,
		0.875000, 0.750000,
		0.937500, 0.750000,
		1.000000, 0.750000,
		0.062500, 0.812500,
		0.000000, 0.812500,
		0.125000, 0.812500,
		0.187500, 0.812500,
		0.250000, 0.812500,
		0.312500, 0.812500,
		0.375000, 0.812500,
		0.437500, 0.812500,
		0.500000, 0.812500,
		0.562500, 0.812500,
		0.625000, 0.812500,
		0.687500, 0.812500,
		0.750000, 0.812500,
		0.812500, 0.812500,
		0.875000, 0.812500,
		0.937500, 0.812500,
		1.000000, 0.812500,
		0.062500, 0.875000,
		0.000000, 0.875000,
		0.125000, 0.875000,
		0.187500, 0.875000,
		0.250000, 0.875000,
		0.312500, 0.875000,
		0.375000, 0.875000,
		0.437500, 0.875000,
		0.500000, 0.875000,
		0.562500, 0.875000,
		0.625000, 0.875000,
		0.687500, 0.875000,
		0.750000, 0.875000,
		0.812500, 0.875000,
		0.875000, 0.875000,
		0.937500, 0.875000,
		1.000000, 0.875000,
		0.062500, 0.937500,
		0.000000, 0.937500,
		0.125000, 0.937500,
		0.187500, 0.937500,
		0.250000, 0.937500,
		0.312500, 0.937500,
		0.375000, 0.937500,
		0.437500, 0.937500,
		0.500000, 0.937500,
		0.562500, 0.937500,
		0.625000, 0.937500,
		0.687500, 0.937500,
		0.750000, 0.937500,
		0.812500, 0.937500,
		0.875000, 0.937500,
		0.937500, 0.937500,
		1.000000, 0.937500,
		0.031250, 1.000000,
		0.093750, 1.000000,
		0.156250, 1.000000,
		0.218750, 1.000000,
		0.281250, 1.000000,
		0.343750, 1.000000,
		0.406250, 1.000000,
		0.468750, 1.000000,
		0.531250, 1.000000,
		0.593750, 1.000000,
		0.656250, 1.000000,
		0.718750, 1.000000,
		0.781250, 1.000000,
		0.843750, 1.000000,
		0.906250, 1.000000,
		0.968750, 1.000000,
	};

	int index[] = {
		0, 1, 2,
		3, 4, 1,
		5, 6, 4,
		7, 8, 6,
		9, 10, 8,
		11, 12, 10,
		13, 14, 12,
		15, 16, 14,
		17, 18, 16,
		19, 20, 18,
		21, 22, 20,
		23, 24, 22,
		25, 26, 24,
		27, 28, 26,
		29, 30, 28,
		31, 32, 30,
		2, 1, 33,
		2, 33, 34,
		1, 4, 35,
		1, 35, 33,
		4, 6, 36,
		4, 36, 35,
		6, 8, 37,
		6, 37, 36,
		8, 10, 38,
		8, 38, 37,
		10, 12, 39,
		10, 39, 38,
		12, 14, 40,
		12, 40, 39,
		14, 16, 41,
		14, 41, 40,
		16, 18, 42,
		16, 42, 41,
		18, 20, 43,
		18, 43, 42,
		20, 22, 44,
		20, 44, 43,
		22, 24, 45,
		22, 45, 44,
		24, 26, 46,
		24, 46, 45,
		26, 28, 47,
		26, 47, 46,
		28, 30, 48,
		28, 48, 47,
		30, 32, 49,
		30, 49, 48,
		34, 33, 50,
		34, 50, 51,
		33, 35, 52,
		33, 52, 50,
		35, 36, 53,
		35, 53, 52,
		36, 37, 54,
		36, 54, 53,
		37, 38, 55,
		37, 55, 54,
		38, 39, 56,
		38, 56, 55,
		39, 40, 57,
		39, 57, 56,
		40, 41, 58,
		40, 58, 57,
		41, 42, 59,
		41, 59, 58,
		42, 43, 60,
		42, 60, 59,
		43, 44, 61,
		43, 61, 60,
		44, 45, 62,
		44, 62, 61,
		45, 46, 63,
		45, 63, 62,
		46, 47, 64,
		46, 64, 63,
		47, 48, 65,
		47, 65, 64,
		48, 49, 66,
		48, 66, 65,
		51, 50, 67,
		51, 67, 68,
		50, 52, 69,
		50, 69, 67,
		52, 53, 70,
		52, 70, 69,
		53, 54, 71,
		53, 71, 70,
		54, 55, 72,
		54, 72, 71,
		55, 56, 73,
		55, 73, 72,
		56, 57, 74,
		56, 74, 73,
		57, 58, 75,
		57, 75, 74,
		58, 59, 76,
		58, 76, 75,
		59, 60, 77,
		59, 77, 76,
		60, 61, 78,
		60, 78, 77,
		61, 62, 79,
		61, 79, 78,
		62, 63, 80,
		62, 80, 79,
		63, 64, 81,
		63, 81, 80,
		64, 65, 82,
		64, 82, 81,
		65, 66, 83,
		65, 83, 82,
		68, 67, 84,
		68, 84, 85,
		67, 69, 86,
		67, 86, 84,
		69, 70, 87,
		69, 87, 86,
		70, 71, 88,
		70, 88, 87,
		71, 72, 89,
		71, 89, 88,
		72, 73, 90,
		72, 90, 89,
		73, 74, 91,
		73, 91, 90,
		74, 75, 92,
		74, 92, 91,
		75, 76, 93,
		75, 93, 92,
		76, 77, 94,
		76, 94, 93,
		77, 78, 95,
		77, 95, 94,
		78, 79, 96,
		78, 96, 95,
		79, 80, 97,
		79, 97, 96,
		80, 81, 98,
		80, 98, 97,
		81, 82, 99,
		81, 99, 98,
		82, 83, 100,
		82, 100, 99,
		85, 84, 101,
		85, 101, 102,
		84, 86, 103,
		84, 103, 101,
		86, 87, 104,
		86, 104, 103,
		87, 88, 105,
		87, 105, 104,
		88, 89, 106,
		88, 106, 105,
		89, 90, 107,
		89, 107, 106,
		90, 91, 108,
		90, 108, 107,
		91, 92, 109,
		91, 109, 108,
		92, 93, 110,
		92, 110, 109,
		93, 94, 111,
		93, 111, 110,
		94, 95, 112,
		94, 112, 111,
		95, 96, 113,
		95, 113, 112,
		96, 97, 114,
		96, 114, 113,
		97, 98, 115,
		97, 115, 114,
		98, 99, 116,
		98, 116, 115,
		99, 100, 117,
		99, 117, 116,
		102, 101, 118,
		102, 118, 119,
		101, 103, 120,
		101, 120, 118,
		103, 104, 121,
		103, 121, 120,
		104, 105, 122,
		104, 122, 121,
		105, 106, 123,
		105, 123, 122,
		106, 107, 124,
		106, 124, 123,
		107, 108, 125,
		107, 125, 124,
		108, 109, 126,
		108, 126, 125,
		109, 110, 127,
		109, 127, 126,
		110, 111, 128,
		110, 128, 127,
		111, 112, 129,
		111, 129, 128,
		112, 113, 130,
		112, 130, 129,
		113, 114, 131,
		113, 131, 130,
		114, 115, 132,
		114, 132, 131,
		115, 116, 133,
		115, 133, 132,
		116, 117, 134,
		116, 134, 133,
		119, 118, 135,
		119, 135, 136,
		118, 120, 137,
		118, 137, 135,
		120, 121, 138,
		120, 138, 137,
		121, 122, 139,
		121, 139, 138,
		122, 123, 140,
		122, 140, 139,
		123, 124, 141,
		123, 141, 140,
		124, 125, 142,
		124, 142, 141,
		125, 126, 143,
		125, 143, 142,
		126, 127, 144,
		126, 144, 143,
		127, 128, 145,
		127, 145, 144,
		128, 129, 146,
		128, 146, 145,
		129, 130, 147,
		129, 147, 146,
		130, 131, 148,
		130, 148, 147,
		131, 132, 149,
		131, 149, 148,
		132, 133, 150,
		132, 150, 149,
		133, 134, 151,
		133, 151, 150,
		136, 135, 152,
		136, 152, 153,
		135, 137, 154,
		135, 154, 152,
		137, 138, 155,
		137, 155, 154,
		138, 139, 156,
		138, 156, 155,
		139, 140, 157,
		139, 157, 156,
		140, 141, 158,
		140, 158, 157,
		141, 142, 159,
		141, 159, 158,
		142, 143, 160,
		142, 160, 159,
		143, 144, 161,
		143, 161, 160,
		144, 145, 162,
		144, 162, 161,
		145, 146, 163,
		145, 163, 162,
		146, 147, 164,
		146, 164, 163,
		147, 148, 165,
		147, 165, 164,
		148, 149, 166,
		148, 166, 165,
		149, 150, 167,
		149, 167, 166,
		150, 151, 168,
		150, 168, 167,
		153, 152, 169,
		153, 169, 170,
		152, 154, 171,
		152, 171, 169,
		154, 155, 172,
		154, 172, 171,
		155, 156, 173,
		155, 173, 172,
		156, 157, 174,
		156, 174, 173,
		157, 158, 175,
		157, 175, 174,
		158, 159, 176,
		158, 176, 175,
		159, 160, 177,
		159, 177, 176,
		160, 161, 178,
		160, 178, 177,
		161, 162, 179,
		161, 179, 178,
		162, 163, 180,
		162, 180, 179,
		163, 164, 181,
		163, 181, 180,
		164, 165, 182,
		164, 182, 181,
		165, 166, 183,
		165, 183, 182,
		166, 167, 184,
		166, 184, 183,
		167, 168, 185,
		167, 185, 184,
		170, 169, 186,
		170, 186, 187,
		169, 171, 188,
		169, 188, 186,
		171, 172, 189,
		171, 189, 188,
		172, 173, 190,
		172, 190, 189,
		173, 174, 191,
		173, 191, 190,
		174, 175, 192,
		174, 192, 191,
		175, 176, 193,
		175, 193, 192,
		176, 177, 194,
		176, 194, 193,
		177, 178, 195,
		177, 195, 194,
		178, 179, 196,
		178, 196, 195,
		179, 180, 197,
		179, 197, 196,
		180, 181, 198,
		180, 198, 197,
		181, 182, 199,
		181, 199, 198,
		182, 183, 200,
		182, 200, 199,
		183, 184, 201,
		183, 201, 200,
		184, 185, 202,
		184, 202, 201,
		187, 186, 203,
		187, 203, 204,
		186, 188, 205,
		186, 205, 203,
		188, 189, 206,
		188, 206, 205,
		189, 190, 207,
		189, 207, 206,
		190, 191, 208,
		190, 208, 207,
		191, 192, 209,
		191, 209, 208,
		192, 193, 210,
		192, 210, 209,
		193, 194, 211,
		193, 211, 210,
		194, 195, 212,
		194, 212, 211,
		195, 196, 213,
		195, 213, 212,
		196, 197, 214,
		196, 214, 213,
		197, 198, 215,
		197, 215, 214,
		198, 199, 216,
		198, 216, 215,
		199, 200, 217,
		199, 217, 216,
		200, 201, 218,
		200, 218, 217,
		201, 202, 219,
		201, 219, 218,
		204, 203, 220,
		204, 220, 221,
		203, 205, 222,
		203, 222, 220,
		205, 206, 223,
		205, 223, 222,
		206, 207, 224,
		206, 224, 223,
		207, 208, 225,
		207, 225, 224,
		208, 209, 226,
		208, 226, 225,
		209, 210, 227,
		209, 227, 226,
		210, 211, 228,
		210, 228, 227,
		211, 212, 229,
		211, 229, 228,
		212, 213, 230,
		212, 230, 229,
		213, 214, 231,
		213, 231, 230,
		214, 215, 232,
		214, 232, 231,
		215, 216, 233,
		215, 233, 232,
		216, 217, 234,
		216, 234, 233,
		217, 218, 235,
		217, 235, 234,
		218, 219, 236,
		218, 236, 235,
		221, 220, 237,
		221, 237, 238,
		220, 222, 239,
		220, 239, 237,
		222, 223, 240,
		222, 240, 239,
		223, 224, 241,
		223, 241, 240,
		224, 225, 242,
		224, 242, 241,
		225, 226, 243,
		225, 243, 242,
		226, 227, 244,
		226, 244, 243,
		227, 228, 245,
		227, 245, 244,
		228, 229, 246,
		228, 246, 245,
		229, 230, 247,
		229, 247, 246,
		230, 231, 248,
		230, 248, 247,
		231, 232, 249,
		231, 249, 248,
		232, 233, 250,
		232, 250, 249,
		233, 234, 251,
		233, 251, 250,
		234, 235, 252,
		234, 252, 251,
		235, 236, 253,
		235, 253, 252,
		238, 237, 254,
		238, 254, 255,
		237, 239, 256,
		237, 256, 254,
		239, 240, 257,
		239, 257, 256,
		240, 241, 258,
		240, 258, 257,
		241, 242, 259,
		241, 259, 258,
		242, 243, 260,
		242, 260, 259,
		243, 244, 261,
		243, 261, 260,
		244, 245, 262,
		244, 262, 261,
		245, 246, 263,
		245, 263, 262,
		246, 247, 264,
		246, 264, 263,
		247, 248, 265,
		247, 265, 264,
		248, 249, 266,
		248, 266, 265,
		249, 250, 267,
		249, 267, 266,
		250, 251, 268,
		250, 268, 267,
		251, 252, 269,
		251, 269, 268,
		252, 253, 270,
		252, 270, 269,
		255, 254, 271,
		254, 256, 272,
		256, 257, 273,
		257, 258, 274,
		258, 259, 275,
		259, 260, 276,
		260, 261, 277,
		261, 262, 278,
		262, 263, 279,
		263, 264, 280,
		264, 265, 281,
		265, 266, 282,
		266, 267, 283,
		267, 268, 284,
		268, 269, 285,
		269, 270, 286,
	};

	ModelCreator::ModelParam param;
	param.Reset();

	param.pos = pos;
	param.norm = normal;
	param.uv = uv;

	param.numVertices = 287;

	param.index = index;
	param.numIndex = 480;


	param.name = "DefaultSphere";

	const int id = 0;

	model->pArray[id] = new TemplateList<ModelBase*>;

	model->pArray[id]->obj = ModelCreator::Create(param, pDevice);

	model->pArray[id]->thisNumber = id;

	model->list.Push_Back(model->pArray[id]);

}
