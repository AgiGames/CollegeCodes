#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>
#include <conio.h>

/*
User defined class which contains the values input by the user.
A method exists which can be called to take inputs.
*/
class input{
public:
    float leftBoundary;
    float rightBoundary;
    int noOfGridPoints;
    float diffusionCoeff;
    float timeStep;
    float gridStep;

    void TakeInputs(){
        std::cout << "Enter the left boundary condition of the X array: ";
        std::cin >> leftBoundary;
        std::cout << "\n";
        std::cout << "Enter the right boundary condition of the X array: ";
        std::cin >> rightBoundary;
        std::cout << "\n";
        std::cout << "Enter the total number of grid points of the X array: ";
        std::cin >> noOfGridPoints;
        std::cout << "\n";
        std::cout << "Enter the diffusion coefficient (a): ";
        std::cin >> diffusionCoeff;
        std::cout << "\n";
        std::cout << "Enter the time step (delta t): ";
        std::cin >> timeStep;
        std::cout << "\n";
        std::cout << "Enter the grid step (delta x): ";
        std::cin >> gridStep;
        std::cout << "\n";
    }
};

//Method to print a vector with the name of the vector.
void PrintVector(std::vector<float>* vector, std::string str, input* inputs){
    std::cout << str << "\n";
    for (int i = 0; i < inputs->noOfGridPoints; i++)
        std::cout << std::fixed << std::setprecision(3) << (*vector)[i] << " ";
    std::cout << "\n\n";
}

/*
The linspace method fills the XValues array.
Uses oundary condition, right boundary condition and total number of gridpoint from input class object reference.
*/
std::vector<float> Linspace(std::vector<float> values, input* inputs){
    float increment = inputs->rightBoundary / ((inputs->noOfGridPoints) - 1);
    values[0] = inputs->leftBoundary; 
    for(int i = 1; i < inputs->noOfGridPoints; i++){
        values[i] = values[i-1] + increment;
    }
    return values;
}

/*
Function for finding the predictor step for a given vector.
Formula of MacCormack method used from wikipedia.
Takes initial values, vector that will contain predicted values and input class object reference respectively.
*/
std::vector<float> PredictorStep(std::vector<float> values, std::vector<float> predValues, input* inputs){
    predValues[0] = values[0];
    predValues[inputs->noOfGridPoints - 1] =  values[inputs->noOfGridPoints - 1];
    for (int i = 1; i < inputs->noOfGridPoints - 1; i++)
        predValues[i] = values[i] - (inputs->diffusionCoeff * (inputs->timeStep/inputs->gridStep) * (values[i+1] - values[i]));
    return predValues;
}

/*
Function for finding the corrector step for a given vector.
Formula of MacCormack method used from wikipedia.
Takes initial values, predicted values, vector that will contain corrected values and input class object reference respectively.
*/
std::vector<float> CorrectorStep(std::vector<float> values, std::vector<float> predValues, std::vector<float> corrValues, input* inputs){
    corrValues[0] = values[0];
    corrValues[inputs->noOfGridPoints - 1] = values[inputs->noOfGridPoints - 1];
    for (int i = 1; i < inputs->noOfGridPoints - 1; i++)
        corrValues[i] = (0.5 * (values[i] + predValues[i])) - ((inputs->diffusionCoeff / 2) * (inputs->timeStep / inputs->gridStep) * (predValues[i+1] - predValues[i-1])); 
    return corrValues;
}

/*
The first and last values of predicted and corrected values are manually assigned.
This is because they are boundary conditions, whose values do not change from the initial values.
*/

//Main driver code.
int main(){
    input inputs;
    inputs.TakeInputs();
    
    //XValues -> Original X Values of the grid calculated from given boundary conditions for X.
    std::string vectorName = "X VALUES:";
    std::vector<float> XValues(inputs.noOfGridPoints);
    XValues = Linspace(XValues, &inputs);
    PrintVector(&XValues, vectorName, &inputs);

    //PValues -> Values of pressure calculated from X Values. (P(x) = 1 - (0.3146x))
    vectorName = "P VALUES:";
    std::vector<float> PValues(inputs.noOfGridPoints);
    for (int i = 0; i < inputs.noOfGridPoints; i++) //Calculates P Values.
        PValues[i] = 1 - (0.3146 * XValues[i]);
    PrintVector(&PValues, vectorName, &inputs);

    //predPValues -> Values of pressure predicted by the MacCormack predictor step.
    vectorName = "PREDICTED P VALUES:";
    std::vector<float> predPValues(inputs.noOfGridPoints);
    predPValues = PredictorStep(PValues, predPValues, &inputs);
    PrintVector(&predPValues, vectorName, &inputs);

    //corrPValues -> Values of pressure corrected by the MacCormack corrector step.
    vectorName = "CORRECTED P VALUES:";
    std::vector<float> corrPValues(inputs.noOfGridPoints);
    corrPValues = CorrectorStep(PValues, predPValues, corrPValues, &inputs);
    PrintVector(&corrPValues, vectorName, &inputs);

    //TValues -> Values of temperature calculated from X Values. (T(x) = 1 - (0.2314x))
    vectorName = "T VALUES:";
    std::vector<float> TValues(inputs.noOfGridPoints);
    for (int i = 0; i < inputs.noOfGridPoints; i++) //Calculates T Values.
        TValues[i] = 1 - (0.2314 * XValues[i]);
    PrintVector(&TValues, vectorName, &inputs);

    //predTValues -> Values of temperature predicted by the MacCormack predictor step.
    vectorName = "PREDICTED T VALUES:";
    std::vector<float> predTValues(inputs.noOfGridPoints);
    predTValues = PredictorStep(TValues, predTValues, &inputs);
    PrintVector(&predTValues, vectorName, &inputs);

    //corrTValues -> values of temperature corrected by the MacCormack corrector step.
    vectorName = "CORRECTED T VALUES:";
    std::vector<float> corrTValues(inputs.noOfGridPoints);
    corrTValues = CorrectorStep(TValues, predTValues, corrTValues, &inputs);
    PrintVector(&corrTValues, vectorName, &inputs);

    //VValues -> Values of velocity calculated from X Values and corrected T Values. ((0.1 + 1.09x) * pow(corrected T, 0.5))
    vectorName = "V VALUES:";
    std::vector<float> VValues(inputs.noOfGridPoints);
    for (int i = 0; i < inputs.noOfGridPoints; i++) //Calculates V Values.
        VValues[i] = (0.1 + (1.09 * XValues[i])) * pow(corrTValues[i], 0.5);
    PrintVector(&VValues, vectorName, &inputs);  //Some values may print as 'nan' as they are imaginary.

    getch();

    return 0;
}