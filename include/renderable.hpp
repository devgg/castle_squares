#pragma once

class renderable {
public:
	virtual ~renderable() {}
	virtual void render() = 0;
};