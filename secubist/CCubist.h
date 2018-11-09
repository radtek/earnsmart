/* 
 * File:   CCubist.h
 * Author: santony
 *
 * Created on December 3, 2013, 5:41 PM
 */

#ifndef CCUBIST_H
#define	CCUBIST_H

#include "CAttribute.h"
#include "COptions.h"
#include "defns.h"
#include <vector>
#include <map>

using namespace std;

namespace se
{
    namespace math
    {
        typedef vector<CAttribute>                        Attributes;
        typedef map<string, Attributes::iterator>         AttributesByName;
        
        class CCubist 
        {
            friend class COptions;
            
        public:
            CCubist();
            CCubist(const CCubist& orig);
            virtual ~CCubist();
            
            void AddAttribute(CAttribute & attribute, bool IsTargetAttribute = false);
            
            void BeginLoadTrainData(U32 samplecount = 0, bool Train = true, bool AllowUnknownTarget=false);      // begin load training data
            void BeginAddRecord();     // call this first, then call FeedAttrValues method for each attribute including target (last one)
            void EndAddRecord();       // and end with this call.
            bool EndLoadTrainData();         // end with loading training data.
            void Reassess();            // cubist model is rebuilt using existing as well as any newly added data.

            void BeginPredict();        // call this first, then call FeedAttrValues method for each attribute including target (last one)
            R8 EndPredict();          // and end with this call. Returns predicted value in the attribute same as target attribute

            void FeedAttrValues(string targetName, string value);   // discrete
            void FeedAttrValues(string targetName, R8 value);       // continuous
            void FeedAttrValues(string targetName, DATETIME value); // datetime
            void FeedAttrValues(string targetName, CDate value);     // date
            
            CAttribute& get_Attribute(string& name);
            CAttribute& get_TargetAttribute();
            
            COptions& get_Options();
            
            const string & get_RuleText() const;     // returns the rules and the formula per rule.
            
            // this is a sample. the function content
            // needs to copied in your function stack to make this work.
            void EnlargeRTStack();  // increase the runtime stack for intense memory work.

        private:
            // read the attributes and then construct a matching
            // datastructure as used by cubist lib.
            ErrorId ApplyAttributeName(CAttribute& attribute);
            
            ErrorId VerifyAndSetAtributeValue(Attribute Att, CSTR value);

            // throws seexception if name is not found in the attributes added using AddAttribute method.
            void SetTarget(string const& name); 
            
            
        private:
            
            enum FreeMask
            {
                FM_None = 0x0,
                FM_Cttee = 0x1,
                FM_LastCase = 0x2,
                FM_AttMsg = 0x4,
            };
            
            Attributes          _attributes;
            AttributesByName    _attrByName;
            string              _targetAttribute;
            COptions*            _options;
            I32                 _enHasFreed;
            string              _ruleDescription;
            
            /*************************************************************************/
            /*									 */
            /*		Data defined in global.c				 */
            /*		------------------------				 */
            /*									 */
            /*************************************************************************/

            //Construct, save and evaluate a single model
            //double SingleCttee(void);
            
            Attribute
                        ClassAtt, /* attribute to use as class */
                        LabelAtt,/* attribute to use as case ID */
                        CWtAtt;/* attribute to use as case weight */

             char	*IgnoredVals;/* values of labels and atts marked ignore */
             int	IValsSize,/* size of above */
                        IValsOffset;/* index of first free char */

            int	MaxAtt,/* max att number */
                        MaxDiscrVal,/* max discrete values for any att */
                        Precision,/* decimal places for target */
                        MaxLabel,/* max characters in case label */
                        LineNo,/* input line number */
                        ErrMsgs,/* errors found */
                        AttExIn,/* attribute exclusions/inclusions */
                        TSBase;/* base day for time stamps */

            CaseNo	MaxCase;/* max data item number */

            DataRec *Case;/* data items */

            DataRec *SaveCase,/* original case order for better caching */
                        *Blocked;/* cross-validation blocks */
            CaseNo	SaveMaxCase;/* original number of cases  */

            DiscrValue
                        *MaxAttVal,/* number of values for each att */
                        *Modal;/* most frequent value for discr att */

            char	*SpecialStatus;/* special att treatment */

            Definition
                        *AttDef;/* definitions of implicit atts */
            Attribute
                        **AttDefUses;/* list of attributes used by definition */

            __String	*AttName,/* att names */
                        **AttValName;/* att value names */
            

            FILE	*Of;/* output file */
            __String	FileStem;

            ContValue
                        *AttMean,/* means of att values */
                        *AttSD,/* std dev ditto */
                        *AttMaxVal,/* maximum value in training data */
                        *AttMinVal,/* minimum ditto */
                        *AttPref,/* preference value */
                        Ceiling,/* max allowable global prediction */
                        Floor,/* min allowable global prediction */
                        AvCWt;/* average case weight */

            float	ErrReduction;/* benefit of committee model */

            double   *AttUnit;/* units in which attribute reported */

            int	*AttPrec;/* Attribute precision  */

            DataRec *Instance,/* training cases */
                        Ref[2];/* reference points */
            CaseNo	MaxInstance;/* highest instance */
            Index	KDTree;/* index of same */
            NNEnvRec
                        GNNEnv;/* global NN environment */
            float	*RSPredVal;/* tabulated RS predictions */

            EnvRec	GEnv;/* global environment */

            Tree	TempMT;/* intermediate model tree */

            SortRec	*SRec;/* cache for sorting */

            float	GlobalMean,/* mean of entire training set  */
                        GlobalSD,/* std dev of entire training set */
                        GlobalErr;/* av abs error over training set */

            /*************************************************************************/
            /*									 */
            /*	  Global data for Cubist used for building model trees		 */
            /*	  ----------------------------------------------------		 */
            /*									 */
            /*************************************************************************/
            
            char	Fn[512];/* file name */

            FILE	*Mf,/* file for saving models  */
                        *Pf;/* file for predicted test values */

            /*************************************************************************/
            /*									 */
            /*	Global data for constructing and applying rules			 */
            /*      -----------------------------------------------			 */
            /*									 */
            /*************************************************************************/
            
            CRule	*Rule;/* current rules */
            RuleNo	NRules;/* number of rules */
            int	RuleSpace;/* space currently allocated for rules */

            RRuleSet
                        *Cttee;

            /*************************************************************************/
            /*									 */
            /*		Global parameters for Cubist				 */
            /*		----------------------------				 */
            /*									 */
            /*************************************************************************/
            

            int	VERBOSITY,/* verbosity level (0 = none) */
                        FOLDS,/* cross-validation folds */
                        NN,/* nearest neighbors to use */
                        MEMBERS;/* members in committee */

            float	MAXD;/* max distance for close neighbors */

            Boolean	XVAL,/* true if perform crossvalidation */
                        CHOOSEMODE,/* choose whether to use instances */
                        USEINSTANCES,/* using instances */
                        UNBIASED;/* correct any rule bias */

            float	SAMPLE;/* sample training proportion */
            int	KRInit;/* KRandom initializer for SAMPLE */
            Boolean	LOCK;/* true if sample locked */

            CaseCount
                        MINITEMS;/* min rule coverage */
            int	MAXRULES;/* max number of rules */

            float	EXTRAP;/* allowed extrapolation from models */

            Boolean SuppressErrorMessages;
            
            Boolean	Sorted;


            #define Inc 2048

            /*  Alternative random number generator  */
            #define AltRandom	drand48()

            //double drand48(void);

            double		*Total,		/* [Condition] */
                    *PredErr,		/* [Condition] */
                    *Model;

            Boolean		**CondFailedBy,	/* [Condition][CaseNo] */
                    *Deleted;		/* [Condition] */

            Condition	*Stack;

            int		Leaves,
                    MaxDepth,
                    NCond,
                    Bestd;

            short		*NFail,		/* NFail[i] = conditions failed by i */
                    *LocalNFail;	/* copy used during rule pruning */

            CaseNo		*Succ,		/* case following case i */
                    Fail0,			/* first satisfying all conditions */
                    Fail1,			/* ditto all but one */
                    FailMany;		/* first case failing 2+ conditions */

            float		*CPredVal;		/* raw model values for each case */

            CaseNo	SampleFrom;		/* file count for sampling */
            
            
            int	Stage;		/* Current stage number  */
            FILE	*Uf;			/* File to which update info written  */
            
            CaseCount	SumCases, *SumCond, *SumModel;
            Boolean		*AttUsed=0;

            int	Delimiter;
            char	LineBuffer[MAXLINEBUFFER], *LBp;
            
            char	*Buff;			/* buffer for input characters */
            int	BuffSize, BN;		/* size and index of next character */

            EltRec	*TStack;		/* expression stack model */
            int	TStackSize, TSN;	/* size of stack and index of next entry */

            int	DefSize, DN;		/* size of definition and next element */

            Boolean PreviousError;		/* to avoid parasitic errors */

            AttValue _UNK,			/* quasi-constant for unknown value */
                 _NA;			/* ditto for not applicable */
            
            int	 MinN;			/* minimum close neighbors */

            int	 Try;			/* sample size for estimates */
            Boolean	 UseAll,		/* true if no sampling */
                 SetNN,			/* true if NN set automatically */
                 *Tested;		/* for BuildIndex */
            CaseCount
                 *ValFreq;		/* ditto */

            AttValue *KDBlock;		/* copy of instances in KDTree order to
                               improve caching  */
            
            int	Entry;

            char*	Prop[32]=
                {"null",
                    "id",
                    "att",
                    "elts",
                    "prec",
                    "globalmean",
                    "floor",
                    "ceiling",
                    "sample",
                    "init",
                    "mean",
                    "sd",
                    "mode",
                    "entries",
                    "rules",
                    "cover",
                    "loval",
                    "hival",
                    "extrap",
                    "insts",
                    "nn",
                    "maxd",
                    "esterr",
                    "conds",
                    "type",
                    "cut",
                    "result",
                    "val",
                    "coeff",
                    "max",
                    "min",
                    "redn"
                       };
            char	
                PropName[20],
                *PropVal,
                *Unquoted;
            int	PropValSize;         
            
            double	TotalErr, ExtraErr, TotalParams, ExtraParams, AdjErrLim, NewAdjErr;

            Tree	Weakest;
            
            /*  If lines look like getting too long while a tree is being
                printed, subtrees are broken off and printed separately after
                the main tree is finished	 */

            int	SubTree,		/* highest subtree to be printed */
                SubSpace;		/* maximum subtree encountered */
            Tree	*SubDef;		/* pointers to subtrees */
            Boolean	LastBranch[Width];	/* whether printing last branch of subtree */
            
            typedef	struct _datablockrec
                {
                  DataRec	Head;		/* first address */
                  int		Allocated;	/* number of cases in this block */
                  _datablockrec*	Prev;		/* previous data block */
                }
                DataBlockRec;

            typedef struct _datablockrec	*DataBlock;

            DataBlock	DataMem;
            int		DataBlockSize;
            
            
        private:
            
            void	    SingleCttee(void);
            Boolean	    ConstructCttee(__String modelText);
            RRuleSet    ConstructRuleSet(int ModelNo);
            void	    EvaluateCttee(RRuleSet *RS, Boolean Details);
            void	    SampleTrainingCases(void);
            void	    AttributeUsage(void);
            void	    UpdateUsage(CRule R);
            void	    NoteUsed(Attribute Att);

                /* getnames.c */

            Boolean	    ReadName(FILE *f, __String s, int n, char ColonOpt);
            void	    GetNames(FILE *Nf);
            void	    ExplicitAtt(FILE *Nf);
            int	    Which(__String Val, __String *List, int First, int Last);
            void	    ListAttsUsed(void);
            void	    FreeNamesData(void);
            int	    InChar(FILE *f);

                /* implicitatt.c */

            void	    ImplicitAtt(FILE *Nf);
            void	    ReadDefinition(FILE *f);
            void	    Append(char c);
            Boolean	    Expression(void);
            Boolean	    Conjunct(void);
            Boolean	    SExpression(void);
            Boolean	    AExpression(void);
            Boolean	    Term(void);
            Boolean	    Factor(void);
            Boolean	    Primary(void);
            Boolean	    Atom(void);
            Boolean	    Find(__String S);
            int	    FindOne(__String *Alt);
            Attribute   FindAttName(void);
            void	    DefSyntaxError(__String Msg);
            void	    DefSemanticsError(int Fi, __String Msg, int OpCode);
            void	    Dump(char OpCode, ContValue F, __String S, int Fi);
            void	    DumpOp(char OpCode, int Fi);
            Boolean	    UpdateTStack(char OpCode, ContValue F, __String S, int Fi);
            AttValue    EvaluateDef(Definition D, DataRec Case);

                /* getdata.c */

            void	    GetData(FILE *Df, Boolean Train, Boolean AllowUnknownTarget);
            Boolean	    ReplaceUnknowns(DataRec Case, Boolean *AttMsg);
            DataRec	    GetDataRec(FILE *Df, Boolean Train);
            CaseNo	    CountData(FILE *Df);
            int	    StoreIVal(__String S);
            void	    FreeData(DataRec *Case);
            void	    CheckValue(DataRec Case, Attribute Att);
            void	    FindLimits(Attribute Att, ContValue *Min, ContValue *Max);

                /* predict.c */

            float	    PredictValue(RRuleSet *Cttee, DataRec CaseDesc);
            float	    RuleSetPrediction(RRuleSet RS, DataRec CaseDesc);
            Boolean	    Matches(CRule R, DataRec Case);
            float	    LinModel(double *Model, DataRec Case);
            float	    RawLinModel(double *Model, DataRec Case);
            void	    FindPredictedValues(RRuleSet *RS, CaseNo Fp, CaseNo Lp);

                /* formtree.c */

            void	    InitialiseEnvData(void);
            void	    FreeEnvData(void);
            void	    FindGlobalProperties(void);
            void	    FormTree(CaseNo, CaseNo, int, Tree *, Tree);
            void	    AddModels(CaseNo Fp, CaseNo Lp, Tree T, Tree Parent);
            CaseNo	    Group(DiscrValue, CaseNo, CaseNo, Tree);
            void	    Divide(Tree Node, CaseNo Fp, CaseNo Lp, int Level);
            void	    AddSplitAtts(Tree T);
            void	    AddDefAtts(void);
            void	    FindModelAtts(double *Model);

                /* discr.c */

            void	    EvalDiscreteAtt(Tree, Attribute, CaseNo Fp, CaseNo Lp);
            void	    EvalBinarySplit(Tree, Attribute, CaseNo Fp, CaseNo Lp);
            void	    EvalSubsetSplit(Tree, Attribute, CaseNo Fp, CaseNo Lp);
            void	    DiscreteTest(Tree Node, Attribute Att, __Set *Subset);

                /* contin.c */

            void	    EvalContinuousAtt(Tree, Attribute, CaseNo Fp, CaseNo Lp);
            void	    ContinTest(Tree Node, Attribute Att, float Cut);
            void	    AdjustAllThresholds(Tree T);
            void	    AdjustThresholds(Tree T, Attribute Att);
            ContValue   GreatestValueBelow(ContValue Th);

                /* sort.c */

            void	    Cachesort(CaseNo Fp, CaseNo Lp);

                /* trees.c */

            void	    FindDepth(Tree T);
            void	    PrintTree(Tree T, __String Title);
            void	    Show(Tree T, int Sh);
            void	    ShowBranch(int Sh, Tree T, DiscrValue v, DiscrValue BrNo);
            int	    MaxLine(Tree SubTree);
            void	    Indent(int Sh, int BrNo);
            void	    FreeTree(Tree T);
            Tree	    Leaf(CaseCount Cases, double Mean, double SD);
            void	    Sprout(Tree T, DiscrValue Branches);
            int	    TreeSize(Tree T);
            int	    TreeLeaves(Tree T);

                /* prunetree.c */

            void	    Prune(Tree T);
            void	    SetProperties(Tree T, Tree Parent);
            void	    UnsproutAndUpdate(Tree Pruned, double ExtraErr, double ExtraParams);
            void	    Unsprout(Tree T);
            void	    SmoothModels(Tree T, Tree Parent, CaseNo Fp, CaseNo Lp);
            double	    ErrVariance(double *Model, CaseNo Fp, CaseNo Lp, double *Err);
            double	    Smooth(double PVal, int PCases, double LVal, int LCases);
            void	    FindErrors(Tree T, CaseNo Fp, CaseNo Lp);
            void	    CValToStr(ContValue CV, Attribute Att, __String DS);
            Boolean	    FindWeakestSubtree(Tree T);
            float	    TreeValue(Tree T, DataRec Case);
            float	    MedianResid(CaseNo Fp, CaseNo Lp, double Want);

                /* regress.c */

            void	    Regress(CaseNo Fp, CaseNo Lp, double *Model);
            void	    BuildTables(CaseNo Fp, CaseNo Lp);
            void	    Solve(double *Model);
            void	    FindActiveAtts(void);
            void	    AddRow(double *Model, short From, short To, double Factor);
            void	    ExchangeRow(double *Model, short From, short To);
            int	    CountCoeffs(double *Model);
            void	    SimplifyModel(DataRec *D, CaseNo Fp, CaseNo Lp, double *Model);

                /* stats.c */

            double	    AverageDev(float Mean, CaseNo Fp, CaseNo Lp);
            double	    SD(double N, double Sum, double SumSq);
            double	    ComputeGain(Tree Node);
            double	    AverageErr(DataRec *D, CaseNo Fp, CaseNo Lp, double *Model);
            double	    EstimateErr(double Val, double NData, float NParam);

                /* utility.c */

            void	    PrintHeader(__String Title);
            char	    ProcessOption(int Argc, char **Argv, char *Str);
            void	    *Pmalloc(size_t Bytes);
            void	    *Prealloc(void *Present, size_t Bytes);
            void	    *Pcalloc(size_t Number, unsigned int Size);
            DataRec	    NewCase(void);
            void	    FreeCases(void);
            void	    FreeLastCase(DataRec Case);
            void	    FreeVector(void **V, int First, int Last);
            double	    KRandom(void);
            void	    ResetKR(int KRInit);
            void	    Error(int ErrNo, __String S1, __String S2);
            __String	    CaseLabel(CaseNo N);
            FILE *	    GetFile(__String Extension, __String RW);
            double	    ExecTime(void);
            int	    Denominator(ContValue Val);
            int	    FracBase(Attribute Att);
            int	    GetInt(__String S, int N);
            int	    DateToDay(__String DS);
            void	    DayToDate(int DI, __String Date);
            int	    TimeToSecs(__String TS);
            void	    SecsToTime(int Secs, __String Time);
            void	    SetTSBase(int y);
            int	    TStampToMins(__String TS);
            void	    Check(float Val, float Low, float High);
            void	    Cleanup(void);
            #ifdef UTF8
            int	    UTF8CharWidth(unsigned char *U);
            int	    wcwidth(wchar_t ucs);
            int	    wcswidth(const wchar_t *pwcs, size_t n);
            #endif

                /* formrules.c */

            RRuleSet    FormRules(Tree T);
            void	    TreeParameters(Tree T, int D);
            void	    Scan(Tree T);
            void	    PushCondition(void);
            void	    PopCondition(void);
            void	    PruneRule(Condition Cond[], float InitCoeffs);
            void	    UpdateCount(int d, CaseNo i, double *Total, double *PredErr);
            void	    ProcessLists(void);
            void	    AddToList(CaseNo *List, CaseNo N);
            void	    DeleteFromList(CaseNo *Before, CaseNo N);
            int	    SingleFail(CaseNo i);
            void	    RemoveBias(CRule R, int Coeffs);
            void	    OrderRules(void);
            void	    FreeFormRuleData(void);


                /* rules.c */

            Boolean	    NewRule(Condition Cond[], int NConds, Boolean *Deleted,
                        CaseNo Cover, float Mean, float LoVal, float HiVal,
                        float EstErr, double *Model);
            Boolean	    SameRule(RuleNo r, Condition Cond[], int NConds);
            void	    ReleaseRule(CRule R);
            void	    FreeCttee(RRuleSet *Cttee);
            void	    PrintRules(RRuleSet, __String);
            void	    PrintRule(CRule R);
            void	    PrintCondition(Condition C);
            Boolean	    Satisfies(DataRec CaseDesc, Condition OneCond);

                /* modelfiles.c */

            void	    CheckFile(__String Extension, Boolean Write);
            void	    WriteFilePrefix(__String Extension);
            void	    ReadFilePrefix(__String Extension);
            void	    SaveDiscreteNames(void);
            void	    SaveCommittee(RRuleSet *Cttee, __String modelText);
            void	    SaveRules(RRuleSet RS, __String modelText);
            void	    AsciiOut(__String Pre, __String S);
            void	    AsciiOut1(__String Pre, __String S, __String modelText);
            void	    ReadHeader(void);
            RRuleSet    *GetCommittee(__String Extension);
            RRuleSet    InRules(void);
            CRule	    InRule(void);
            Condition   InCondition(void);
            int	    ReadProp(char *Delim);
            __String	    RemoveQuotes(__String S);
            __Set	    MakeSubset(Attribute Att);

                /* update.c (Unix) or frontend.c (WIN32) */

            void	    NotifyStage(int);
            void	    Progress(float);

                /* instance.c */

            Boolean	    InitialiseInstances(RRuleSet *Cttee);
            Boolean	    SetParameters(RRuleSet *Cttee);
            void	    CheckForms(RRuleSet *Cttee);
            void	    CopyInstances(void);
            float	    NNEstimate(RRuleSet *Cttee, DataRec Case);
            float	    Distance(DataRec Case1, DataRec Case2, float Thresh);
            void	    CheckDistance(DataRec Case, CaseNo Saved);
            void	    FindNearestNeighbors(DataRec Case);
            float	    AverageNeighbors(RRuleSet *Cttee, DataRec Case);
            Index	    BuildIndex(CaseNo Fp, CaseNo Lp);
            void	    ScanIndex(DataRec Case, Index Node, float MinD);
            void	    SwapInstance(CaseNo A, CaseNo B);
            void	    FreeIndex(Index Node);
            void	    FreeInstances(void);

                /*  xval.c  */

            void	    CrossVal(void);
            void	    Prepare(void);
            void	    Shuffle(long *);
            void	    Summary(void);
            
            
            
        };
        
    }
}

#endif	/* CCUBIST_H */

