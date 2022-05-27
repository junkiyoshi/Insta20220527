#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(0);
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);
	ofSetLineWidth(2);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
}

//--------------------------------------------------------------
void ofApp::draw() {

	// Day 228 Zenith Blue
	vector<ofColor> color_palette;
	color_palette.push_back(ofColor(134, 179, 224));
	color_palette.push_back(ofColor(251, 218, 205));
	color_palette.push_back(ofColor(234, 96, 148));

	ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);

	int number_of_satellite = 3;
	int number_of_log = 80;
	int location_radius = 200;
	auto speed = 1.5;
	ofColor color(color_palette[0]);

	for (int param = 0; param < 360; param += 180) {

		auto base_location = glm::vec3(location_radius * cos((ofGetFrameNum() * speed + param + number_of_log) * DEG_TO_RAD), location_radius * sin((ofGetFrameNum() * speed + param + number_of_log) * DEG_TO_RAD), 0);
		auto base_radius = 60;

		for (int i = 0; i < number_of_satellite; i++) {

			ofSetColor(color_palette[i]);

			float noise_seed_x = ofRandom(1000);
			float noise_seed_y = ofRandom(1000);
			auto location = glm::vec3(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1));
			location = glm::normalize(location) * base_radius * 1.5;
			vector<glm::vec3> log_list;
			for (int k = 0; k < number_of_log; k++) {

				auto rotation_y = glm::rotate(glm::mat4(), ofMap(ofNoise(noise_seed_y, (ofGetFrameNum() * speed + k) * 0.005), 0, 1, -PI * 2, PI * 2), glm::vec3(0, 1, 0));
				auto rotation_x = glm::rotate(glm::mat4(), ofMap(ofNoise(noise_seed_x, (ofGetFrameNum() * speed + k) * 0.005), 0, 1, -PI * 2, PI * 2), glm::vec3(1, 0, 0));
				auto tmp_base_location = glm::vec3(location_radius * cos((ofGetFrameNum() * speed + param + k) * DEG_TO_RAD), location_radius * sin((ofGetFrameNum() * speed + param + k) * DEG_TO_RAD), 0);
				auto log_location = tmp_base_location + glm::vec4(location, 0) * rotation_y * rotation_x;
				log_list.push_back(log_location);
			}

			auto head_size = 12;
			ofMesh mesh;
			vector<glm::vec3> right, left;
			glm::vec3 last_location;
			float last_theta;

			for (int k = 0; k < log_list.size() - 1; k++) {

				auto location = log_list[k];
				auto next = log_list[k + 1];

				auto direction = next - location;
				auto theta = atan2(direction.y, direction.x);

				right.push_back(location + glm::vec3(ofMap(k, 0, log_list.size(), 0, head_size) * cos(theta + PI * 0.5), ofMap(k, 0, log_list.size(), 0, head_size) * sin(theta + PI * 0.5), 0));
				left.push_back(location + glm::vec3(ofMap(k, 0, log_list.size(), 0, head_size) * cos(theta - PI * 0.5), ofMap(k, 0, log_list.size(), 0, head_size) * sin(theta - PI * 0.5), 0));

				last_location = location;
				last_theta = theta;
			}

			for (int k = 0; k < right.size(); k++) {

				mesh.addVertex(left[k]);
				mesh.addVertex(right[k]);

				mesh.addColor(color_palette[i]);
				mesh.addColor(color_palette[i]);
			}

			for (int k = 0; k < mesh.getNumVertices() - 2; k += 2) {

				mesh.addIndex(k + 0); mesh.addIndex(k + 1); mesh.addIndex(k + 3);
				mesh.addIndex(k + 0); mesh.addIndex(k + 2); mesh.addIndex(k + 3);
			}

			mesh.addVertex(last_location);
			mesh.addColor(color_palette[i]);
			int index = mesh.getNumVertices();
			for (auto theta = last_theta - PI * 0.5; theta <= last_theta + PI * 0.5; theta += PI / 20) {

				mesh.addVertex(last_location + glm::vec3(head_size * cos(theta), head_size * sin(theta), 0));
				mesh.addColor(ofColor(color_palette[i]));
			}

			for (int k = index; k < mesh.getNumVertices() - 1; k++) {

				mesh.addIndex(index); mesh.addIndex(k + 0); mesh.addIndex(k + 1);
			}

			mesh.draw();
		}
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}