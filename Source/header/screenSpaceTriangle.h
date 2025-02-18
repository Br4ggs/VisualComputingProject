#ifndef SCREENSPACETRI_H
#define SCREENSPACETRI_H

//simple pythagorean triangle rendered directly to screen space. for testing purposes
class ScreenSpaceTriangle
{
public:
	ScreenSpaceTriangle(const ShaderProgram* program);

	void render() const;
	void destroy();

private:
	const ShaderProgram* program;
};

#endif // !SCREENSPACETRI_H
