/*
 *  reactiveSineLine.h
 *  
 *
 *  Created by Christopher Pietsch on 20.11.11.
 *  Copyright 2011 chrispie.com. All rights reserved.
 *
 */

#include "ofMain.h"
#include "MSAPhysics2D.h"
#include "ofxSimpleGuiToo.h"

using namespace MSA;

class reactiveSineLine {
public:
	
	
	int width;
	int height;
	
	Physics::World2D		physics;
	Physics::Particle2D		*animatedLineParticle;
	
	vector<Physics::Particle2D*> lineParticles;
	vector<Physics::Spring2D*> lineSprings;
	
	vector<ofPoint> line;
	
	
	int linePosY;
	
	float lpMass;
	float lpBounce;
	int lpSteps;
	int lpRadius;
	
	float lsStrength;
	float lsRLength;
	float lsMinLength;
	float lsMaxLength;
	float lsNeighbourStrength;
	float lpDragL;
	float lpDragRStart;
	float lpDragREnd;
	int aniLPposX;
	int middleParticlePos;
	float aniSpeed;
	
	int lineBridgeSize;
	bool lpUpdate;
	
	float bsStrength,bsRLength,bsDrag;
	
	
	// FUNCTIONS
	
	reactiveSineLine(){
	
	}
	
	void setup(){
		
		width = ofGetWidth();
		height = ofGetHeight();
		
		linePosY=height/2;
		
		lpMass =1;
		lpBounce=1.1f;
		lpSteps=2;
		lpRadius=0;
		
		lsStrength=0.045f;
		lsRLength=0.85f;
		lsMinLength=0;
		lsMaxLength=17;
		lsNeighbourStrength=-1.35f;
		lpDragL=0.24f;
		lpDragRStart=1.0f;
		lpDragREnd=0.985f;
		aniLPposX=300;
		middleParticlePos=20;
		aniSpeed=10;
		
		lineBridgeSize=4;
		lpUpdate=false;
		
		bsStrength=3.30f;
		bsRLength=-2.6f;
		bsDrag=-2.6;
		lpUpdate=false;
		
		
		//physics.verbose = true;			// dump activity to log
		physics.setGravity(Vec2f(0, 0));
		
		// set world dimensions, not essential, but speeds up collision
		physics.setWorldSize(Vec2f(0,0), Vec2f(width, height));
		physics.setSectorCount(1);
		physics.setDrag(1.0f);
		physics.enableCollision();
		
		initScene();
		
		// TODO GUI HIER REIN
		
		
	}
	
	void setupGUI(ofxSimpleGuiPage* gui) {
		
		gui->addTitle("Particles");
		gui->addSlider("Line BirdgeSize", lineBridgeSize, 0, 50); 
		//gui->addSlider("Line PosY", linePosY, 0, height);
		gui->addSlider("Animated PosX", aniLPposX, 0, width); 
		gui->addSlider("Animated Speed", aniSpeed, 0, 50); 
		gui->addSlider("Steps", lpSteps, 0, 20); 
		gui->addSlider("Mass", lpMass, 0.0, 1); 
		gui->addSlider("Bounce", lpBounce, 0.0, 10); 
		gui->addSlider("Radius", lpRadius, 0, 20); 
		
		gui->addTitle("Drag");
		gui->addSlider("Drag left", lpDragL, 0, 1); 
		gui->addSlider("Drag right start", lpDragRStart, 0.5, 1); 
		gui->addSlider("Drag right end", lpDragREnd, 0.5, 1); 
		
		gui->addTitle("Spring");
		gui->addSlider("S Strength", lsStrength, 0.0, 3); 
		gui->addSlider("S NeighbourStrength", lsNeighbourStrength, -5, 5); 
		gui->addSlider("S Rest Legnth", lsRLength, 0, 10); 
		gui->addSlider("S MinLength", lsMinLength, 0, 100); 
		gui->addSlider("S MaxLength", lsMaxLength, 0, 100); 
		
		gui->addTitle("Bridge");
		gui->addSlider("B Strength", bsStrength, 0.0, 3); 
		gui->addSlider("B RLength", bsRLength, 0.0, 30); 
		gui->addSlider("B Drag", bsDrag, -10, 10); 
		gui->addToggle("updateLine", lpUpdate);

		
	}
	
	
	void update(float smoothedVol){
		physics.update();

		sineGenerator(smoothedVol);
		
		if(lpUpdate){
			initScene();
			lpUpdate=false;
		}
		
		
	}
	
	vector<ofPoint>& getLine(){
		
		ofPolyline bridge;
		bool added=false;
		
		bridge.addVertex(ofPoint(lineParticles[middleParticlePos-lineBridgeSize]->getPosition().x,
								 lineParticles[middleParticlePos-lineBridgeSize]->getPosition().y
								 ));
		
		float  x1 = lineParticles[middleParticlePos-lineBridgeSize]->getPosition().x;
		float  y1 = lineParticles[middleParticlePos-lineBridgeSize]->getPosition().y;
		float  x2 = lineParticles[middleParticlePos-1]->getPosition().x;
		float  y2 = lineParticles[middleParticlePos-1]->getPosition().y;
		float  x3 = lineParticles[middleParticlePos+lineBridgeSize+1]->getPosition().x;
		float  y3 = lineParticles[middleParticlePos+lineBridgeSize+1]->getPosition().y;
		float  res = lineBridgeSize/lpSteps ;

		bridge.bezierTo(x1,y1,x2,y2,x3,y3,res);
		
		bridge.addVertex(ofPoint(lineParticles[middleParticlePos+lineBridgeSize]->getPosition().x,
								 lineParticles[middleParticlePos+lineBridgeSize]->getPosition().y
								 ));
		
		
		
		line.clear();
		line.reserve(lineParticles.size()+lineBridgeSize+2);
		
		for(int i=0; i<lineParticles.size(); i++) {
			if(i>middleParticlePos-lineBridgeSize && i<middleParticlePos+lineBridgeSize){
				
				if(!added){
					for(int a=0; a<bridge.size(); a++) {
						line.push_back(ofPoint(bridge[a].x,bridge[a].y- linePosY));
					}
					added=true;
				}
				
				
			} else {
				line.push_back(ofPoint(lineParticles[i]->getPosition().x,lineParticles[i]->getPosition().y- linePosY));
			}
		}
		
		
		return line;
	}
	
	void draw(){
		
		ofPushStyle();
		ofNoFill();
		ofSetRectMode(OF_RECTMODE_CENTER);
		ofPushMatrix();
		ofTranslate(0, 0, 0);
		
		ofSetColor(100, 200, 100,100);
		for(int i=0; i<physics.numberOfParticles(); i++) {
			Physics::Particle2D *p = physics.getParticle(i);		
			ofRect(p->getPosition().x, p->getPosition().y,p->getRadius()+1,p->getRadius()+1);
		}
		
		ofSetColor(0, 200, 200,255);
		for(int i=0; i<lineSprings.size(); i++) {
			Physics::Spring2D *spring = (Physics::Spring2D *) lineSprings[i];
			Physics::Particle2D *a = spring->getOneEnd();
			Physics::Particle2D *b = spring->getTheOtherEnd();
			
			if(i>middleParticlePos-lineBridgeSize && i<middleParticlePos+lineBridgeSize){
				
			} else {
				ofLine(a->getPosition().x, a->getPosition().y, b->getPosition().x, b->getPosition().y);
			}
			
			//ofLine(a->getPosition().x, a->getPosition().y, b->getPosition().x, b->getPosition().y);
			
		}
		
		ofBeginShape();
		ofVertex(lineParticles[middleParticlePos-lineBridgeSize]->getPosition().x,
				 lineParticles[middleParticlePos-lineBridgeSize]->getPosition().y);
		
		ofBezierVertex(
					   lineParticles[middleParticlePos-lineBridgeSize]->getPosition().x,
					   lineParticles[middleParticlePos-lineBridgeSize]->getPosition().y,
					   lineParticles[middleParticlePos-1]->getPosition().x,
					   lineParticles[middleParticlePos-1]->getPosition().y,
					   lineParticles[middleParticlePos+lineBridgeSize+1]->getPosition().x,
					   lineParticles[middleParticlePos+lineBridgeSize+1]->getPosition().y
					   );
		
		ofVertex(lineParticles[middleParticlePos+lineBridgeSize]->getPosition().x,
				 lineParticles[middleParticlePos+lineBridgeSize]->getPosition().y);
		ofEndShape();
		
		ofPopMatrix();
		ofPopStyle();
		
	}
	
protected:
	
	void makeLine() {
		middleParticlePos=aniLPposX/lpSteps;
		int lineLength=width/lpSteps;
		
		for (int i =0; i<lineLength; i++) {
			Physics::Particle2D *a = physics.makeParticle(Vec2f(i*lpSteps,linePosY), lpMass);
			a->setBounce(lpBounce)->setRadius(lpRadius)->enableCollision()->makeFree();
			
			float drag;
			if(i<middleParticlePos){
				drag=ofMap(i, 0, middleParticlePos, lpDragL, 1);
			} else {
				drag=ofMap(i, middleParticlePos, lineLength, lpDragRStart, lpDragREnd);
			}
			a->setDrag(drag);
			
			if (i>0) {
				Physics::Particle2D *b = physics.getParticle(i-1);
				Physics::Spring2D *s = physics.makeSpring(a, b, lsStrength, lsRLength);
				lineSprings.push_back(s);
			}
			
			if(i==0 || i==lineLength-1){
				a->makeFixed();
			}
			lineParticles.push_back(a);
		}
		
		
		animatedLineParticle =lineParticles[middleParticlePos];
		animatedLineParticle->makeFixed()->disableCollision();
		
	}
	
	
	void makeStrongBridge() {
		Physics::Spring2D *neighbourS;
		Physics::Particle2D *neighbourP;
		float strength;
		int length=5;
		
		for (int i=-1*length; i<length; i++) {
			
			neighbourP=lineParticles[middleParticlePos+i];
			strength=neighbourP->getDrag()+lsNeighbourStrength*((length-abs(i))/(float)length);
			neighbourP->setDrag(strength);
			/*
			 neighbourS=lineSprings[middleParticlePos+i];
			 strength=neighbourS->getStrength()+lsNeighbourStrength*((length-abs(i))/(float)length);
			 neighbourS->setStrength(strength);
			 */
			
		}
		
	}
	
	
	void initScene() {
		
		physics.clear();
		lineParticles.clear();
		lineSprings.clear();
		
		makeLine();
		//makeStrongBridge();
	}
	
	
	void sineGenerator(float smoothedVol){
		
		float scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);
		
		//int y= sin(ofGetFrameNum()/aniSpeed)*(20+buffer+scaledVol*200)+linePosY;
		int y= sin(ofGetFrameNum()/aniSpeed)*(20+scaledVol*200)+linePosY;
		y=y*(1-scaledVol);
		
		animatedLineParticle->moveTo(Vec2f(animatedLineParticle->getPosition().x,y));
		
	}
	
	
};


