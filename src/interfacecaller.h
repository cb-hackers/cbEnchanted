#ifndef INTERFACECALLER_H
#define INTERFACECALLER_H
class InterfaceSystem;
class CBVariableHolder;
class InterfaceCaller
{
	public:
		InterfaceCaller(CBVariableHolder *s);
		bool init();


		void commandDrawScreen();

	private:
		InterfaceSystem *system;
		CBVariableHolder *stack;
};

#endif // INTERFACECALLER_H
