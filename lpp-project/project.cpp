#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

/**
 * TESTING PROBLEM
 * MAX z= 2x1+3x2
 *  S.to x1+2X2 <= 2
 *       2x2 +x1<=3
 * x1,x2>=0
*/

/**
 * TESTING PROBLEM BIG-M
 * MAX z= 1x1+5x2
 *  S.to 4x1 + 5x2 <= 6
 *       1x1 + 3x2 >= 2
 * x1,x2>=0
 */ 



void displayVector(vector<int>);
void displayVector(vector<double>, double);
void displayinitialtable(vector<double>,double,vector<vector<double>>,vector<double>,vector <int>);
void displayTable(vector<double>,double,vector<vector<double>>,vector<double>,vector <int>, int, int);
const char separator = ' ';
const int numWidth = 10;

class LPP
{
public:
    double optimalSolution{}; //stores optimal solution at each simplex table
    double temp;              //stores the optimal solution of Max problem for later conversion to Min .
    int enteringVariable{};   //stores index of the entering variable in indexOfBasic vector
    int leavingVariable{};    //stores index of the entering variable in objective vector
    int problemType;          //stores index of the entering variable in objective vector
    double pivotElement;      //stores value of the pivot element at each simplex table
    vector<int> indexOfBasic; //stores index of current basic variables in objective vector

    void setProblemType(int probType)
    {
        problemType = probType;
    }

    /**
     * Checks initial basic variables  by reading the constraints
     * and adds thier variable in indexOfBaisc vector.
     * @param constraint: 2D constraints vector.
     */
    void checkBasic(vector<vector<double>> constraint, vector<double> objective, vector<double> resource)
    {
        int BasicInt{}, flag{1};
        // i will denote the column of the table
        for (int i = 0; i < constraint[0].size(); i++)
        {
            // j will denote the row of the table
            BasicInt = 0;
            flag = 1;
            for (int j = 0; j < constraint.size(); j++)
            {
                if (constraint[j][i] < 0)
                    flag = 0;
                BasicInt += constraint[j][i];
            }
            if (flag && (BasicInt == 1))
                indexOfBasic.push_back(i);
        }
        displayinitialtable(objective, optimalSolution, constraint, resource, indexOfBasic);
    }

    void displayinitialtable(vector<double> object, double sol, vector<vector<double>> constr, vector<double> resi, vector<int> basics)
    {
        cout << left << setw(10) << "Basic";
        for (int i = 1; i <= 5; i++)
            cout << "x" << setw(10) << i;
        cout << "Solution\n";
        cout << left << setw(10) << setfill(separator) << "Z";
        displayVector(object, sol); // display new objective row element

        for (int j = 0; j < constr.size(); j++)
        {
            cout << "x" << left << setw(9) << basics[j] + 1;
            displayVector(constr[j], resi[j]);
        }
    }

    void displayBasicVariables(vector<double> resource)
    {

        for (int i = 0; i < indexOfBasic.size(); i++)
        {
            cout << "x" << indexOfBasic[i] + 1 << " : " << resource[i] << endl;
        }
    }

    /**
     * Checks for the most negative/positive element in the Z-row
     * based on type of lpp problem max/min.
     * @param objRow: Objective row vector.
     * By default problem type is 1 that is maximizarion problem.
     */
    int checkEnteringVar(vector<double> objRow, int problemType = 1)
    {
        int enteringVarIndex = 0;
        for (int i = 0; i < objRow.size(); i++)
        {
            if (objRow[i] < objRow[enteringVarIndex])
                enteringVarIndex = i;
            // else
            // {
            //     if (objRow[i] > objRow[enteringVarIndex])
            //         enteringVarIndex = i;
            // }
        }
        enteringVariable = enteringVarIndex;
        cout << "Entering variable is: x" << enteringVariable + 1 << endl;
        return enteringVariable;
    }

    /**
     * @param res: Resource Vector
     * @param constraint: 2D constraint Vector
     * Takes resource and constraint vectors and prints leaving variable
     * declaring and initialisation var to store minimum ratio
     * a variable which will store all ratios
     * to calculate the ratios we have to select particular entries of the rows ,
     * so we need this variable to store index of  the same.
     * We make use of a for loop which starts from zero to size of res vector,
     * then we pass it through a if block which checks whether it is greater than zero or not.
     * if it is >0 ratio is calculated.
     * Another if block determine the least of ratios obtained and assign it as min
     * ratio and the corresponding value of "i" is given to leaving variable and then
     * we print the leaving variable
     */
    int checkleavingVariable(vector<double> reso, vector<vector<double>> constraint)
    {
        double min_ratio = 0;
        double ratio;
        double currentVar;
        for (int i = 0; i < reso.size(); i++)
        {
            currentVar = constraint[i][enteringVariable];
            if (currentVar > 0)
            {
                ratio = reso[i] / currentVar;
                if (min_ratio == 0 || ratio < min_ratio)
                {
                    min_ratio = ratio;
                    leavingVariable = i;
                }
            }
        }
        cout << "Leaving variable is: x" << indexOfBasic[leavingVariable] + 1 << endl; //printing leaving variable.
        int returnLeave = indexOfBasic[leavingVariable];
        indexOfBasic[leavingVariable] = enteringVariable;
        return returnLeave;
    }

    /**
     * Sets the pivot element from the constraints vector
     * based on entering and leaving variable indexes
     * @param constraints: 2D vector of all constraints
     */
    void setPivot(vector<vector<double>> constraints)
    {
        pivotElement = constraints[leavingVariable][enteringVariable];
        cout << "Pivot element is: " << pivotElement << endl<<endl;
    }

    /**
     * The optimality is acheived if every element in the objective row is greater than 0 for maximization problem here
     * The vector objective stores the Z row elements of the problem for every iteration.
     * We run a loop to check for every element in the vector objective.
     * If all the elements of the vector objective or the Z row are greater than 0, the optimality has reached.
     * If even any one element is not positive, the program continues.
     * @param objective: Objective function vector
     *
     * @return bool: 0 for not optimal and 1 for optimal
     */
    bool checkOptimality(vector<double> objective)
    {
        cout << "\nChecking Optimality...\n";

        for (int i = 0; i < objective.size(); i++)
        {
            if (objective[i] < 0)
            {
                cout << "Optimality not reached\n\n";
                return 0;
            }
        }

        cout << "Optimality reached" << endl;
        return 1;
    }

    /**
    * @param reso: resource vector
    * what does Function? : change elements of old pivot row for next table
    * According to formula : New pivot row = Current pivot row / Pivot element
    * initially it convert the resource element of old pivot row by dividing it by pivotElement
    * and then convert every element of constraint of old pivot row in a for loop by dividing by pivotElement
    */
    void newPivotRow(vector<vector<double>> &constraints, vector<double> &reso)
    {
        reso[leavingVariable] = reso[leavingVariable] / pivotElement;
        for (int i = 0; i < constraints[leavingVariable].size(); i++)
        {
            constraints[leavingVariable][i] = constraints[leavingVariable][i] / pivotElement;
        }
    }

    /**
    * Function newRow
    * What does function do?
    * This function create the new rows of next tabel after calculation of new pivot row
    * According to Formula :   New row element = (Current row element) - (Pivot column coefficient) * (New pivot row coefficient)
    * @param constraint : old constraint 2D vector
    * @param reso : old resource vector
    * @param objective : objective vector
    */
    void newRow(vector<vector<double>> &constraints, vector<double> &objective, vector<double> &reso)
    {
        cout<<"test"<<enteringVariable<<endl;
        double multFactor = 1; //used as a new pivot row coefficient

        for (int j = 0; j < reso.size(); j++)
        {
            if (j == leavingVariable) //skip element at index - leaving variable  i.e pivot row
                continue;             // Reason to skip: already change happend in new pivot row function ie. new pivot row cofficient

            multFactor = constraints[j][enteringVariable] * (-1); // constraints[j][enteringVariable]*(-1) -->  new pivot row coficient

            reso[j] = reso[j] + multFactor * reso[leavingVariable]; // Formula applied and calculate new resource elements
        }

        //Calculate Z row cofficient of resource vector
        multFactor = objective[enteringVariable] * (-1);
        optimalSolution = optimalSolution + multFactor * reso[leavingVariable];
        temp = (-1)*optimalSolution;
        multFactor = objective[enteringVariable] * (-1);

        for (int j = 0; j < objective.size(); j++) // Calculate new objective row elements
        {
            objective[j] = objective[j] + multFactor * constraints[leavingVariable][j]; // formula applied and calculate new objective row
        }

        // displayVector(objective, optimalSolution); // display new objective row element

        multFactor = 1;
        for (int j = 0; j < constraints.size(); j++) // Calcute new Constraint element
        {
            if (j == leavingVariable) continue;
            // skip leaving row elements i.e already change in new pivot row function   

            multFactor = constraints[j][enteringVariable] * (-1);

            for (int i = 0; i < constraints[j].size(); i++)
            {
                constraints[j][i] = constraints[j][i] + multFactor * constraints[leavingVariable][i]; // formula applied and calculate new constraint row
            }
        }
        displayTable(objective,optimalSolution,constraints,reso,indexOfBasic,enteringVariable,leavingVariable);
    }
};

class Constraint : public LPP
{
public:
    vector<vector<double>> constraints{{4, 4}, {1, 3}};

    /**
     * @condition: -1 : <=
     *              0 : =
     *              1 : >=
     */
    vector<int> constype{-1, 1}; //type of constraints ,have to determine constraint type from the user input
    /**
     * What does function do: Displays the constraints in equation form.
     * Example: 2x1 + 3x2 = 4
     * @param res : resource vector
     */
    void display(vector<double> res)
    {
        for (int i = 0; i < constraints.size(); ++i)
        {
            for (int j = 0; j < constraints[i].size(); ++j)
            {
                if (j < (constraints[i].size() - 1))
                    cout << constraints[i][j] << "x" << j + 1 << " + ";
                else
                    cout << constraints[i][j] << "x" << j + 1 << " = " << res[i];
            }
            cout << "\n";
        }
    }

    /**
     * Adds slack or surplus variable in constraint according to
     * type of constraint. This basically converts the constraint
     * into standard form.
     */
    void SlackSurp(vector<double> &objective, int problemType = 1)
    {
        for (int i = 0; i < constraints.size(); ++i)
        {
            if (constype[i] == -1) // -1 : <= enequality
            {
                addZeros(i, 1);         //adds zero for slack var in all except current constraints
                objective.push_back(0); //0 for current slack var
            }
            else if (constype[i] == 1) // 1 : >= enequality
            {
                addZeros(i, -1);        //adds zero for surplus var in all except current constraints
                objective.push_back(0); //0 for current surplus var
                addZeros(i, 1);         //adds zero for artificial var in all except current constraints
                if (problemType == 0)
                    objective.push_back(-1000); //M=1000 for current Artificial var
                else
                    objective.push_back(1000); //M=1000 for current Artificial var
            }
            else
            {
                addZeros(i, 1); //adds zero for artificial var in all except current constraints
                if (problemType == 0)
                    objective.push_back(-1000); //M=1000 for current Artificial var
                else
                    objective.push_back(1000); //M=1000 for current Artificial var
            }
        }
    }

    /**
     * Adds zero in place of slack surplus variables
     * not present in the current constraint.
     * @param index: which constraint out of all is being edited
     * @param coefficient: coefficient to be added in current constraint
     */
    void addZeros(int index, int coefficient)
    {
        for (int i = 0; i < constraints.size(); ++i)
        {
            if (i == index)
                constraints[index].push_back(coefficient);
            else
                constraints[i].push_back(0);
        }
    }
};

class Resource : public LPP
{
public:
    vector<double> reso{6, 2}; //stores resource coefficient
};

class ObjFunc : public LPP
{
public:
    vector<double> objective; //stores objective coefficient.

    //Insert coefficient of objective function to objective vector.
    void Insert()
    {
        objective.push_back(-1);
        objective.push_back(-5);
    }

    /**
     * Displays Objective function.
     * If Min problem first displays Min objective then displays it after converting it into Max form.
     * By default objective parameters are negative .
     * @param coeff : stores the negative value of objective coefficient
     *                if it not equal to 0(to make them positive) otherwise
     *                it stores 0 as it is.
     * Adds '+' between each objective variable except the last one.
     */
    void display()
    {
        int coeff;
        cout << "* LPP Problem *\n\n";
        if (problemType == 1)
        {
            cout << "Max Z = ";
            for (int j = 0; j < objective.size(); j++)
            {

                coeff = (objective[j] != 0) ? (objective[j] * (-1)) : 0;
                // the objective function to be displayed should be in positive form thus we are multiplying by -1
                if (j < (objective.size() - 1))
                {
                    cout << coeff << "x" << j + 1 << " + ";
                }
                else
                {
                    cout << coeff << "x" << j + 1;
                }
            }
        }
        else
        {
            cout << "Min Z = ";
            for (int j = 0; j < objective.size(); j++)
            {
                coeff = (objective[j] != 0) ? (objective[j] * (-1)) : 0;
                // the objective function to be displayed should be in positive form thus we are multiplying by -1
                if (j < (objective.size() - 1))
                {
                    cout << coeff << "x" << j + 1 << " + ";
                }
                else
                {
                    cout << coeff << "x" << j + 1;
                }
            }

            //Converting Minization problem to Maximization problem.
            cout << endl
                 << "Converting the Objective function into Maximization form" << endl;
            cout << "Min (-Z) = ";
            for (int j = 0; j < objective.size(); j++)
            {
                coeff = (objective[j] != 0) ? (objective[j]) : 0;
                if (j < (objective.size() - 1))
                {
                    cout << coeff << "x" << j + 1 << " + ";
                }
                else
                {
                    cout << coeff << "x" << j + 1;
                }
            }
        }
        cout << "\n\n";
    }
};

void bigM_zrow(vector<double> &objective, vector<vector<double>> constraints, vector<int> constype, vector<double> &reso, double &optimalSolution)
{
    // New z row = old z row + (1000*R1 row + 1000*R2 row)

    for (int i = 0; i < constype.size(); i++)
    {
        for (int j = 0; j < objective.size(); j++)
        {
            if (constype[i] == 1)
            {
                objective[j] = objective[j] + (-1000 * constraints[i][j]);
            }
        }
        
        if (constype[i] == 1)
        {
            optimalSolution = optimalSolution + ((-1000) * reso[i]);
        }
    }
}


/**
 * @param dv: Vector containing int type data values.
 * displayVector function prints all the elements
 * of the given vector
*/
void displayVector(vector<int> dv)
{
    for (int i = 0; i < dv.size(); i++)
    {
        cout << left << setw(numWidth) << setfill(separator) << dv[i] << " ";
    }
    cout << endl;
}

// Overloaded displayVector to display a vector containing double type data
void displayVector(vector<double> dv, double resourse)
{
    for (int i = 0; i < dv.size(); i++)
    {
        cout << left << fixed << setprecision(2) << setw(numWidth) << setfill(separator) << dv[i] << " ";
    }
    cout << left << fixed << setprecision(2) << setw(numWidth) << setfill(separator) << resourse << " ";
    cout << endl;
}

void displayTable(vector<double> object ,double sol,vector<vector<double>> constr,vector<double> resi, vector <int> basics, int enteringVariable, int leavingVariable)
    {
        cout<<left<<setw(10)<<"Basic";
        for(int i = 1; i <= 5; i++) cout<<left<<setw(10)<<"x"+to_string(i)<<" ";
        cout<<"Solution\n";
        cout<<left<<setw(10)<<setfill(separator)<<"Z";
        displayVector(object, sol); // display new objective row element
        for (int j = 0; j < constr.size(); j++) // Calcute new Constraint element
        {
            if (j == leavingVariable) // skip leaving row elements i.e already change in new pivot row function
            {   
                cout<<"x"<<left<<setw(9)<<basics[leavingVariable]+1;
                // Displaying Basic Variable Value
                displayVector(constr[leavingVariable], resi[leavingVariable]);
                continue;
            }
            cout<<"x"<<left<<setw(9)<<basics[j]+1;
            // Displaying Basic Variable Value
            displayVector(constr[j], resi[j]); // display new constraint vector
        }
    }       

int main()
{
    int choice;
    cout << "\t\t\t  * LPP Solver *\n\n\n";
    Constraint c;
    ObjFunc o;
    Resource r;
    cout << "Which type of problem is it? Press '1' for Maximization and '0' for Minimization." << endl;
    cin >> choice;

    o.setProblemType(choice); //set min/max problem type
    o.Insert();
    c.SlackSurp(o.objective);
    o.display();
    cout << "Subject To: \n";
    c.display(r.reso);
    cout << endl;

    bigM_zrow(o.objective, c.constraints, c.constype, r.reso, c.optimalSolution);

    //Check initial basic variables
    c.checkBasic(c.constraints, o.objective, r.reso);

    // int a = c.checkOptimality(o.objective);
    int optimalCondition = c.checkOptimality(o.objective);
    while (not optimalCondition)
    {
        c.checkEnteringVar(o.objective);
        c.checkleavingVariable(r.reso, c.constraints);
        c.setPivot(c.constraints);
        c.newPivotRow(c.constraints, r.reso);
        c.newRow(c.constraints, o.objective, r.reso);
        optimalCondition = c.checkOptimality(o.objective);
        cout << endl;
    }

    if (choice == 0)
    {
        cout << "Optimal Solution for Max problem is  : " << c.optimalSolution << endl
             << endl;
        cout << "\nFinal basic variables are: \n";
        c.displayBasicVariables(r.reso);
        cout << "Final Optimal Solution is : " << c.temp << endl
             << endl;
    }
    else
    {
        cout << "Final basic variables are: \n";
        c.displayBasicVariables(r.reso);
        cout << "Optimal Solution is : " << c.optimalSolution << endl
             << endl;
    }

    // getchar();
}