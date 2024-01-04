#include <stdio.h>
#include <math.h>

//A structure containing all variables whos values are given by user.
struct input
{
    float leftBoundary;
    float rightBoundary;
    int noOfGridPoints;
    float diffusionCoeff;
    float timeStep;
    float gridStep;
};

/*
The following array declaration contain values calculated from original input.
XValues -> Original X Values of the grid calculated from given boundary conditions for X.
PValues -> Values of pressure calculated from X Values. (P(x) = 1 - (0.3146x))
predPValues -> Values of pressure predicted by the MacCormack predictor step.
corrPValues -> values of pressure corrected by the MacCormack corrector step.
TValues -> Values of temperature calculated from X Values. (T(x) = 1 - (0.2314x))
predTValues -> Values of temperature predicted by the MacCormack predictor step.
corrTValues -> values of temperature corrected by the MacCormack corrector step.
VValues -> Values of velocity calculated from X Values and corrected T Values. ((0.1 + 1.09x) * pow(corrected T, 0.5))
*/
float XValues[100], PValues[100], predPValues[100], corrPValues[100], TValues[100], predTValues[100], corrTValues[100], VValues[100];

/*
A function of return type struct input. 
Takes inputs for all datapoints in the object created with data type struct input.
*/
struct input GetInputs(struct input inputs){
    printf("Enter the left boundary-condition for X array: ");
    scanf("%f", &inputs.leftBoundary);
    printf("\n");
    printf("Enter the right boundary-condition for X array: ");
    scanf("%f", &inputs.rightBoundary);
    printf("\n");
    printf("Enter total number of grid-points for X array: ");
    scanf("%d", &inputs.noOfGridPoints);
    printf("\n");
    printf("Enter the timestep (delta t): ");
    scanf("%f", &inputs.timeStep);
    printf("\n");
    printf("Enter the gridstep (delta x): ");
    scanf("%f", &inputs.gridStep);
    printf("\n");
    printf("Enter the diffusion-coefficient (a): ");
    scanf("%f", &inputs.diffusionCoeff);
    printf("\n");
    return inputs;
}

/*
The linspace method fills the XValues array.
Takes left boundary condition, right boundary condition and total number of gridpoints.
*/
void linspace(float start, float end, int noOfGridPoints){
    float increment = end / noOfGridPoints;
    XValues[0] = start;
    for (int i = 1; i < noOfGridPoints; i++)
        XValues[i] = XValues[i-1] + increment;
}

//Function for printing a given array.
void PrintArr(float arr[], int noOfGridPoints){
    printf("VALUES: ");
    for(int i = 0; i < noOfGridPoints; i++)
        printf("%.3f ", arr[i]);
    printf("\n");
}

/*
Function for finding the predictor step for a givenn array.
Formula of MacCormack method used from wikipedia.
Takes initial values, address of the first index of array that will contain predicted values and struct of all given inputs respectively.
*/
void PredictorStep(float values[], float* predValues, struct input inputs){
    for (int i = 1; i < (inputs.noOfGridPoints - 1); i++)
        *(predValues + i) = values[i] - (inputs.diffusionCoeff * (inputs.timeStep/inputs.gridStep) * (values[i+1] - values[i]));
}

/*
Function for finding the corrector step for a givenn array.
Formula of MacCormack method used from wikipedia.
Takes initial values, predicted values, address of the first index of array that will contain corrected values struct of all given inputs respectively.
The for loop starts from index 1 and ends at n-2 as the values at index 0 and n-1 are boundary conditions (constant). 
*/
void CorrectorStep(float values[], float predValues[], float* corrValues, struct input inputs){
    for (int i = 1; i < (inputs.noOfGridPoints - 1); i++)
        *(corrValues + i) = (0.5 * (values[i] + predValues[i])) - ((inputs.diffusionCoeff / 2) * (inputs.timeStep / inputs.gridStep) * (predValues[i+1] - predValues[i-1])); 
}

//Main driver code.
int main(){
    struct input inputs;
    inputs = GetInputs(inputs);

    linspace(inputs.leftBoundary, inputs.rightBoundary, inputs.noOfGridPoints);
    printf("\nX VALUES\n");
    PrintArr(XValues, inputs.noOfGridPoints);

    for (int i = 0; i < inputs.noOfGridPoints; i++) //Calculates P Values.
        PValues[i] = 1 - (0.3146 * XValues[i]);
    printf("\nP VALUES\n");
    PrintArr(PValues, inputs.noOfGridPoints);

    for (int i = 0; i < inputs.noOfGridPoints; i++) //Calculates T Values.
        TValues[i] = 1 - (0.2314 * XValues[i]);
    printf("\nT VALUES\n");
    PrintArr(TValues, inputs.noOfGridPoints);

    /*
    The first and last values of predicted and corrected values are manually assigned.
    This is because they are boundary conditions, whose values do not change from the initial values.
    */
    predPValues[0] = PValues[0];
    predPValues[inputs.noOfGridPoints-1] = PValues[inputs.noOfGridPoints-1];
    PredictorStep(PValues, &predPValues[0], inputs);
    printf("\nPredicted P VALUES\n");
    PrintArr(predPValues, inputs.noOfGridPoints);
    corrPValues[0] = PValues[0];
    corrPValues[inputs.noOfGridPoints-1] = PValues[inputs.noOfGridPoints-1];
    CorrectorStep(PValues, predPValues, &corrPValues[0], inputs);
    printf("\nCorrected P VALUES\n");
    PrintArr(corrPValues, inputs.noOfGridPoints);

    predTValues[0] = TValues[0];
    predTValues[inputs.noOfGridPoints-1] = TValues[inputs.noOfGridPoints-1];
    PredictorStep(TValues, &predTValues[0], inputs);
    printf("\nPredicted T VALUES\n");
    corrTValues[0] = TValues[0];
    corrTValues[inputs.noOfGridPoints-1] = TValues[inputs.noOfGridPoints-1];
    PrintArr(predTValues, inputs.noOfGridPoints);
    CorrectorStep(TValues, predTValues, &corrTValues[0], inputs);
    printf("\nCorrected T VALUES\n");
    PrintArr(corrTValues, inputs.noOfGridPoints);

    /*
    The following lines of code calculates and prints V Values.
    Some values may print as 'nan' (unresolved issue for now).
    */
    printf("\nV VALUES\n");
    for (int i = 0; i < inputs.noOfGridPoints; i++) 
        VValues[i] = (0.1 + (1.09 * XValues[i])) * pow(corrTValues[i], 0.5);
    PrintArr(VValues, inputs.noOfGridPoints);

    return 0;
}