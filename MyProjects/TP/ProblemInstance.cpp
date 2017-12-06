#include "ProblemInstance.h"




ProblemInstance::ProblemInstance()
{
	File* fileBanco;

	try
	{
		fileBanco = new File("./Instance/banco");
	} catch (FileNotFound& f)
	{
		cout << "File '" << "./Instance/banco" << "' not found" << endl;
		exit(1);
	}

	Scanner scannerASDist(fileBanco);
	int nVariables = scannerASDist.nextInt();
	vector<vector<double> > inputsValues;

	cout << scannerASDist.nextLine() << endl;
	while (scannerASDist.hasNext())
	{
		vector<double> sampleVar(nVariables);
		for (int v = 0; v < nVariables; v++)
		{
			double vValues = scannerASDist.nextDouble();
			sampleVar[v] = vValues;
		}
		inputsValues.push_back(sampleVar);

		cout<<inputsValues<<endl;
	}

	cout<<"while ended"<<endl;



	inputs = inputsValues;
	operators =
	{	"+", "-", "*", "/"};
	cout << inputs << endl;

}

ProblemInstance::~ProblemInstance()
{
}

int ProblemInstance::getNSamples(){
	return inputs.size();
}
