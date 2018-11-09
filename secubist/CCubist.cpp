/* 
 * File:   CCubist.cpp
 * Author: santony
 * 
 * Created on December 3, 2013, 5:41 PM
 */

#include "internal.h"
#include "CCubist.h"
#include <sstream>
#include <sys/resource.h>

namespace se
{
    namespace math
    {

        int AttCeiling=100;

        char	*Buff;			/* buffer for input characters */
        int	BuffSize, BN;		/* size and index of next character */

        EltRec	*TStack;		/* expression stack model */
        int	TStackSize, TSN;	/* size of stack and index of next entry */

        int	DefSize, DN;		/* size of definition and next element */

        Boolean PreviousError;		/* to avoid parasitic errors */
        
        
        AttValue _UNK,			/* quasi-constant for unknown value */
                 _NA;			/* ditto for not applicable */


        #define FailSyn(Msg)	 {DefSyntaxError(Msg); return false;}
        #define FailSem(Msg)	 {DefSemanticsError(Fi, Msg, OpCode); return false;}

        typedef  union  _xstack_elt
                 {
                    DiscrValue  _discr_val;
                    ContValue   _cont_val;
                    __String      _string_val;
                 }
                 XStackElt;

        #define	cval		_cont_val
        #define	sval		_string_val
        #define	dval		_discr_val

        #ifdef CUBIST
        #define	Unknown(c,a)	(! DVal(c,a))
        #endif

                 
        CaseNo	SampleFrom;		/* file count for sampling */
        CaseNo	CaseSpace, i;
        DataRec	DVec;
        Boolean	AnyUnknown=false, FirstIgnore=true, *AttMsg;
        Attribute	Att;
        ContValue	Val, Range;
        char	CVS[20];
        CaseCount	*Freq;			/* discrete value frequencies */
        CaseNo	Count, WantTrain, LeftTrain, WantTest, LeftTest;
        Boolean	SelectTrain;
        double	Sum, SumSq;
        DiscrValue	v;
        Boolean Train;
        Boolean AllowUnknownTarget;
                 
        
        CCubist::CCubist() : 
            _enHasFreed(FM_None),
                ClassAtt(0),
                LabelAtt(0),
                CWtAtt(0),
                IgnoredVals(0),
                IValsSize(0),
                IValsOffset(0),
                MaxDiscrVal(3),
                MaxLabel(0),
                LineNo(0),
                ErrMsgs(0),
                AttExIn(0),
                TSBase(0),
                MaxCase(1),
                SaveCase(Nil),
                Blocked(Nil),
                MaxAttVal(Nil),
                Modal(Nil),
                SpecialStatus(Nil),
                AttDef(Nil),
                AttDefUses(Nil),
                AttName(Nil),
                AttValName(Nil),
                Of(0),
                FileStem("undefined"),
                AttMean(Nil),
                AttSD(Nil),
                AttMaxVal(Nil),
                AttMinVal(Nil),
                AttPref(Nil),
                ErrReduction(1),
                AttUnit(Nil),
                AttPrec(Nil),
                Instance(Nil),
                //Ref(DataRec[2]),
                MaxInstance(-1),
                KDTree(Nil),
                RSPredVal(Nil),
                TempMT(Nil),
                SRec(Nil),
                //Fn(char[512]),
                Mf(0),
                Pf(0),
                Rule(Nil),
                Cttee(Nil),
                VERBOSITY(0),
                FOLDS(10),
                NN(0),
                MEMBERS(1),
                XVAL(0),
                CHOOSEMODE(0),
                USEINSTANCES(0),
                UNBIASED(0),
                SAMPLE(0.0),
                KRInit(0),
                LOCK(false),
                MAXRULES(100),
                EXTRAP(1.0),
                Total(Nil),
                PredErr(Nil),
                CondFailedBy(Nil),
                Deleted(Nil),
                Stack(Nil),
                NFail(Nil),
                LocalNFail(Nil),
                Succ(Nil),
                CPredVal(Nil),
                Stage(0),
                Uf(0),
                SumCond(0),
                SumModel(0),
                AttUsed(0),
                LBp(LineBuffer),
                Tested(Nil),
                ValFreq(Nil),
                PropVal(Nil),
                PropValSize(0),
                SubSpace(0),
                SubDef(Nil),
                DataMem(Nil),
                DataBlockSize(0)
        {
            _options = new COptions(this);
            ErrMsgs = AttExIn = LineNo  = 0;
            AttName	  = AllocZero(AttCeiling, __String);
            MaxAttVal	  = AllocZero(AttCeiling, DiscrValue);
            AttValName	  = AllocZero(AttCeiling, __String *);
            SpecialStatus = AllocZero(AttCeiling, char);
            AttDef	  = AllocZero(AttCeiling, Definition);
            AttDefUses	  = AllocZero(AttCeiling, Attribute *);
            MaxAtt = LabelAtt = CWtAtt = 0;
        }

        CCubist::CCubist(const CCubist& orig) 
        {
            _attributes = orig._attributes;
            _attrByName = orig._attrByName;
            _targetAttribute = orig._targetAttribute;
            _options = orig._options;
            _enHasFreed = orig._enHasFreed;
        }

        CCubist::~CCubist() 
        {
            if (_enHasFreed & FM_AttMsg)
            {
                Free(AttMsg);					
                AttMsg = Nil;
            }
            if (_enHasFreed & FM_LastCase)
            {
                FreeLastCase(DVec);
            }
            if (_enHasFreed & FM_Cttee)
            {
                FreeCttee(Cttee);
                Cttee = Nil;
            }
            _enHasFreed = FM_None;
            
            delete _options;
        }
        
        COptions& CCubist::get_Options()
        {
            return *_options;
        }
        
        const string& CCubist::get_RuleText() const
        {
            return _ruleDescription;
        }
        
        void CCubist::AddAttribute(CAttribute &attribute, bool IsTargetAttribute)
        {
            if (_attrByName.end() != _attrByName.find(attribute.get_Name()))
                throw seexception("Duplicate attribute name.");
            
            _attrByName.insert(make_pair(attribute.get_Name(), _attributes.insert(_attributes.end(), attribute)));
            
            ApplyAttributeName(attribute);

            if (IsTargetAttribute)
            {
                if (attribute.get_DataType() != ADT_Continous)
                    throw seexception("Target attribute must be continous.");

                SetTarget(attribute.get_Name());
            }
        }
        
        void CCubist::SetTarget(string const& name)
        {
            if (_attrByName.find(name) == _attrByName.end())
                throw se::seexception("Target name not part of attibute list.");
            
            _targetAttribute = name;
            
            ClassAtt = Which((__String)name.c_str(), AttName, 1, MaxAtt);
            if ( Skip(ClassAtt) ) SpecialStatus[ClassAtt] -= SKIP;

            if ( MaxAttVal[ClassAtt] > 0 ||
                 StatBit(ClassAtt, DISCRETE|DATEVAL|STIMEVAL|EXCLUDE) )
            {
                throw seexception("Bad target type.");
            }
        }
        
        CAttribute& CCubist::get_Attribute(string& name)
        {
            auto it = _attrByName.find(name);
            if (it != _attrByName.end())
                return *(it->second);
            throw seexception("Attribute not found.");
        }
        
        CAttribute& CCubist::get_TargetAttribute()
        {
            return get_Attribute(_targetAttribute);
        }
        
        void CCubist::EnlargeRTStack()
        {
            struct rlimit rl;
            getrlimit(RLIMIT_STACK, &rl);
            rl.rlim_cur = Max(rl.rlim_cur, 20 * 1024 * 1024);
            if (rl.rlim_max > 0)    // -1 is unlimited
                rl.rlim_cur = Min(rl.rlim_max, rl.rlim_cur);
            setrlimit(RLIMIT_STACK, &rl);
        }

        void CCubist::BeginLoadTrainData(unsigned int count, bool train, bool allowUnknownTarget )
        {
            Train = train;
            allowUnknownTarget = allowUnknownTarget;
            LineNo = 0;
            SuppressErrorMessages = SAMPLE && ! Train;
            if ( Train || ! Case )
            {
                MaxCase = MaxLabel = CaseSpace = 0;
                Case = Alloc(1, DataRec);	/* for error reporting */
            }
            else
            {
                CaseSpace = MaxCase + 1;
                MaxCase++;
            }

            if ( SAMPLE )
            {
                if ( Train )
                {
                    SampleFrom = count;
                    ResetKR(KRInit);		/* initialise KRandom() */
                }
                else
                {
                    ResetKR(KRInit);		/* restore  KRandom() */
                }

                WantTrain = SampleFrom * SAMPLE + 0.5;
                LeftTrain = SampleFrom;

                WantTest  = ( SAMPLE < 0.5 ? WantTrain : SampleFrom - WantTrain );
                LeftTest  = SampleFrom - WantTrain;
            }
            _enHasFreed |= FM_Cttee;
            _enHasFreed |= FM_AttMsg;

        }
        
        void CCubist::BeginAddRecord()
        {
            if (MaxCase < 0)
                throw seexception("BeginLoadTrainData must be called to initialize training state.");
            Case[MaxCase] = DVec = NewCase();
            _enHasFreed |= FM_LastCase;
        }
        
        void CCubist::EndAddRecord()
        {
            int Chars;
            
            Class(DVec) = CVal(DVec, ClassAtt);

            if ( LabelAtt && (Chars = strlen(IgnoredVals + SVal(DVec, LabelAtt))) > MaxLabel )
            {
                MaxLabel = Chars;
            }
            
            if (SAMPLE)
            {
                SelectTrain = KRandom() < WantTrain / (float) LeftTrain --;
                if (SelectTrain)
                    WantTrain --;
                if (SelectTrain != Train || 
                        (!Train && AltRandom >= WantTest / (float)LeftTest--))
                {
                    FreeLastCase(DVec);
                    _enHasFreed &= ~ FM_LastCase;
                    return;
                }
                if (!Train)
                    WantTest--;
            }
            
            if (MaxCase >= CaseSpace)
            {
                CaseSpace += Inc;
                Realloc(Case, CaseSpace+1, DataRec);
            }
            
            if (!NotApplic(DVec, ClassAtt) && (AllowUnknownTarget || CVal(DVec, ClassAtt)!=UNKNOWN))
            {
                Case[MaxCase] = DVec;
                MaxCase++;
            }
        }
        
        bool CCubist::EndLoadTrainData()
        {

            //log4cxx::LoggerPtr _logger = log4cxx::Logger::getLogger("se.cubist.endloadtrainingdata");
            
            FreeLastCase(DVec);
            _enHasFreed &= ~ FM_LastCase;
            
            MaxCase--;
            /*  Replace unknown values by means or modals  */

            if ( AttMean == Nil )
            {
                /*  Find means, limits, modal values and precision  */

                AttMean   = AllocZero(MaxAtt+1, ContValue);
                AttSD     = AllocZero(MaxAtt+1, ContValue);
                AttMaxVal = AllocZero(MaxAtt+1, ContValue);
                AttMinVal = AllocZero(MaxAtt+1, ContValue);
                AttPref   = AllocZero(MaxAtt+1, ContValue);
                AttUnit   = AllocZero(MaxAtt+1, double);
                AttPrec   = AllocZero(MaxAtt+1, int);
                Modal     = Alloc(MaxAtt+1, DiscrValue);
                Freq      = Alloc(MaxDiscrVal+1, CaseCount);

                ForEach(Att, 1, MaxAtt)
                {
                    if ( Exclude(Att) ) continue;

                    if ( Continuous(Att) )
                    {
                        Sum = SumSq = Count = 0;

                        ForEach(i, 0, MaxCase)
                        {
                            Val = CVal(Case[i], Att);
                            if ( Val != UNKNOWN && DVal(Case[i], Att) != NA )
                            {
                                Sum   += Val;
                                SumSq += Val * Val;
                                Count++;
                            }
                        }

                        if ( Count > 1 )
                        {
                            AttPrec[Att] = 10 * FracBase(Att);
                            AttMean[Att] = floor((Sum / Count) * AttPrec[Att] + 0.1) /
                                           AttPrec[Att];
                            AttSD[Att]   = sqrt((SumSq - Sum * Sum / Count) / (Count-1))
                                           + 1E-6;
                        }
                        else
                        {
                            AttPrec[Att] = 0;
                            AttMean[Att] = 0;
                            AttSD[Att]   = 1;
                            SpecialStatus[Att] |= SKIP;
                        }

                        FindLimits(Att, &AttMinVal[Att], &AttMaxVal[Att]);
                    }
                    else
                    {
                        ForEach(v, 0, MaxAttVal[Att])
                        {
                            Freq[v] = 0;
                        }

                        ForEach(i, 0, MaxCase)
                        {
                            if ( (v = DVal(Case[i], Att)) ) Freq[v]++;
                        }

                        Modal[Att] = 0;		/* cannot be N/A */
                        ForEach(v, 2, MaxAttVal[Att])
                        {
                            if ( Freq[v] > Freq[Modal[Att]] ) Modal[Att] = v;
                        }

                        if ( ! Modal[Att] )
                        {
                            SpecialStatus[Att] |= SKIP;
                        }
                    }
                }
                Free(Freq);					Freq = Nil;

                Precision = log(AttPrec[ClassAtt] / 10.0) / log(10.0) + 0.5;
            }
            /*  Now, the actual replacement  */

            AttMsg = AllocZero(MaxAtt+1, Boolean);

            ForEach(i, 0, MaxCase)
            {
                AnyUnknown |= ReplaceUnknowns(Case[i], AttMsg);
            }
            
            if ( Of && Train && AnyUnknown )
            {
                ForEach(Att, 1, MaxAtt)
                {
                    if ( AttMsg[Att] )
                    {
                        if ( Skip(Att) )
                        {
                            continue;
                        }
                        if ( Discrete(Att) )
                        {
                        }
                        else
                        {
                            CValToStr(AttMean[Att], Att, CVS);
                        }
                    }
                }
            }
            if ( Train )
            {
                /*  Find global range and set lowest and highest predicted value  */
                Range = AttMaxVal[ClassAtt] - AttMinVal[ClassAtt];

                Ceiling = AttMaxVal[ClassAtt] + EXTRAP * Range;
                if ( AttMaxVal[ClassAtt] <= 0 && Ceiling > 0 ) Ceiling = 0;

                Floor = AttMinVal[ClassAtt] - EXTRAP * Range;
                if ( AttMinVal[ClassAtt] >= 0 && Floor < 0 ) Floor = 0;
            }

            Free(AttMsg);					
            AttMsg = Nil;
            _enHasFreed &= ~FM_AttMsg;
            
            /*  Check whether case weight attribute appears.  If it does,
                 normalize values to average 1, and replace N/A values and
                 values <= 0 with average value 1  */
            
            double SumCWt = 0;
            CaseNo i, NCWt = 0;

             if ( CWtAtt )
             {
                 fprintf(Of, T_CWtAtt);

                 /*  Find average case weight value  */

                 ForEach(i, 0, MaxCase)
                 {
                     if ( ! NotApplic(Case[i], CWtAtt) && CVal(Case[i], CWtAtt) > 0 )
                     {
                         SumCWt += CVal(Case[i], CWtAtt);
                         NCWt += 1;
                     }
                 }

                 AvCWt = ( NCWt > 0 ? SumCWt / NCWt : 1 );

                 ForEach(i, 0, MaxCase)
                 {
                     if ( ! NotApplic(Case[i], CWtAtt) && CVal(Case[i], CWtAtt) > 0 )
                     {
                         CVal(Case[i], CWtAtt) /= AvCWt;
                     }
                     else
                     {
                         CVal(Case[i], CWtAtt) = 1;
                     }
                 }
             }
             else
             {
                 AvCWt = 1;
             }            

            InitialiseEnvData();

            __String modelText;
            modelText = Alloc(0x4000, char);
            if (!ConstructCttee(modelText))
            {
                Free(modelText);
               // LOGI(_logger, "Failed to construct committee model. Returning.");
                return false;
            }
            
           // LOGI(_logger, "RuleText: " << modelText);
            
            // save model text
            int len = strlen(modelText);
            
            for (int i=0; i!=len; ++i)
            {
                if (isprint(modelText[i]))
                _ruleDescription.push_back(modelText[i]);
            }
            // free string.
            Free(modelText);
            //EvaluateCttee(Cttee, false);
            
            return true;
        }
        
        void CCubist::BeginPredict()
        {
            DVec = NewCase();
        }
        
        R8 CCubist::EndPredict()
        {
            ////
            float result ;
            if (USEINSTANCES)
                result = NNEstimate(Cttee, DVec);
            else
                result = PredictValue(Cttee, DVec);
            
            FreeLastCase(DVec);
            //_enHasFreed &= ~ FM_LastCase;
            //FreeCttee(Cttee);					
            //Cttee = Nil;
            //_enHasFreed &= ~ FM_Cttee;
            return result;
        }
        
        void CCubist::FeedAttrValues(string targetName, string value)   // includes time values
        {
            Attribute Att;
            Att = Which((__String)targetName.c_str(), AttName, 1, MaxAtt);
            
            if (S_False == VerifyAndSetAtributeValue(Att, value.c_str()))
                throw se::seexception("Bad attribute value");
        }
        
        void CCubist::FeedAttrValues(string targetName, double value)
        {
            Attribute Att;
            Att = Which((__String)targetName.c_str(), AttName, 1, MaxAtt);
            
            stringstream strvalue;
            strvalue << value;
            if (S_False == VerifyAndSetAtributeValue(Att, strvalue.str().c_str()))
                throw se::seexception("Bad attribute value");
        }
        
        void CCubist::FeedAttrValues(string targetName, DATETIME value)
        {
            Attribute Att;
            Att = Which((__String)targetName.c_str(), AttName, 1, MaxAtt);
            
            SP<IString> pStr;
            value.ToString(&pStr);
            if (S_False == VerifyAndSetAtributeValue(Att, pStr->get_Buffer()))
                throw se::seexception("Bad attribute value");
            
        }
        
        void CCubist::FeedAttrValues(string targetName, CDate value)
        {
            Attribute Att;
            Att = Which((__String)targetName.c_str(), AttName, 1, MaxAtt);

            SP<IString> pStr;
            value.ToString(&pStr);
            if (S_False == VerifyAndSetAtributeValue(Att, pStr->get_Buffer()))
                throw se::seexception("Bad attribute value");
        }
        
        /************Private Methods**************/
        
        ErrorId CCubist::VerifyAndSetAtributeValue(Attribute Att, CSTR value)
        {
            int Dv;
            ContValue	Cv;
            if (AttDef[Att])
            {
                DVec[Att] = EvaluateDef(AttDef[Att], DVec);
                if (Continuous(Att))
                {
                    CheckValue(DVec, Att);
                }
                return S_Success;
            }
            if (Exclude(Att))
            {
                if ( Att == LabelAtt )
                {
                    /*  Record the value as a string  */
                    stringstream str;
                    str << value;
                    SVal(DVec,Att) = StoreIVal((STR)value);
                }
                return S_Success;
            }
            if (!strcmp(value, "?"))
            {
                /*  Unknown value  */
                if ( Continuous(Att) )
                {
                    CVal(DVec, Att) = UNKNOWN;
                }
                else
                {
                    DVal(DVec, Att) = 0;
                }
                return S_Success;
            }
	    if ( ! strcmp(value, "N/A") )
	    {
		/*  Non-applicable value  */

		DVal(DVec, Att) = NA;
                return S_Success;
	    }
	    if ( Discrete(Att) )
	    {
		Dv = Which((STR)value, AttValName[Att], 1, MaxAttVal[Att]);
		if ( ! Dv )
		{
		    if ( StatBit(Att, DISCRETE) )
		    {
			if ( Train || XVAL )
			{
			    /*  Add value to list  */
			    if ( MaxAttVal[Att] >= (long) AttValName[Att][0] )
			    {
				//XError(TOOMANYVALS, AttName[Att], (char *) AttValName[Att][0] - 1);
				Dv = MaxAttVal[Att];
			    }
			    else
			    {
				Dv = ++MaxAttVal[Att];
				AttValName[Att][Dv]   = strdup(value);
				AttValName[Att][Dv+1] = (STR)"<other>"; /* no free */
			    }
			    if ( Dv > MaxDiscrVal )	       
			    {	       
			       MaxDiscrVal = Dv;
			    }			     
			}
			else
			{
			    /*  Set value to "<other>"  */
			    Dv = MaxAttVal[Att] + 1;
			}
		    }
		    else
		    {
			throw se::seexception("Bad attribute.");
		    }
		}
		DVal(DVec, Att) = Dv;
                return S_Success;
	    }            
	    else
	    {
		/*  Continuous value  */
		if ( TStampVal(Att) )
		{
		    Cv = TStampToMins((STR)value);
		    if ( Cv >= 1E9 )	/* long time in future */
		    {
			//XError(BADTSTMP, AttName[Att], (STR)value);
			Cv = UNKNOWN;
		    }
		}
		else
		if ( DateVal(Att) )
		{
		    Cv = DateToDay((STR)value);
		    if ( Cv < 1 )
		    {
			//XError(BADDATE, AttName[Att], Name);
			Cv = UNKNOWN;
		    }
		}
		else
		if ( TimeVal(Att) )
		{
		    Cv = TimeToSecs((STR)value);
		    if ( Cv < 0 )
		    {
			//XError(BADTIME, AttName[Att], Name);
			Cv = UNKNOWN;
		    }
		}
		else
		{
		    Cv = strtod((STR)value, 0);
		}
		CVal(DVec, Att) = Cv;
		CheckValue(DVec, Att);
                return S_Success;
	    }            
            return S_False;
        }

        
        ErrorId CCubist::ApplyAttributeName(CAttribute& attribute)
        {
            try
            {
                Attribute Att;
                Att = Which((__String)attribute.get_Name().c_str(), AttName, 1, MaxAtt);
//                AttExIn = attribute.get_DataType() == ADT_Ignore ? -1 : 1 ;
//                if ( AttExIn == 0 )
//                {
//                    ForEach(Att, 1, MaxAtt)
//                    {
//                        SpecialStatus[Att] |= SKIP;
//                    }
//                }
                // ATTR POSITION
                //attrPos = _attrByName[attribute.get_Name()] - _attributes.begin();
		//Att = Which(attribute.get_Name().c_str(), AttName, 1, MaxAtt);
                
//                if (AttExIn == 1)
//                    SpecialStatus[Att] -= SKIP;
//                else
//                    SpecialStatus[Att] |= SKIP;
                
                if ( ++MaxAtt >= AttCeiling )
                {
                    AttCeiling += 100;
                    Realloc(AttName, AttCeiling, __String);
                    Realloc(MaxAttVal, AttCeiling, DiscrValue);
                    Realloc(AttValName, AttCeiling, __String *);
                    Realloc(SpecialStatus, AttCeiling, char);
                    Realloc(AttDef, AttCeiling, Definition);
                    Realloc(AttDefUses, AttCeiling, Attribute *);
                }
                
                AttName[MaxAtt]	      = strdup(attribute.get_Name().c_str());
                SpecialStatus[MaxAtt] = 0;
                AttDef[MaxAtt]	      = Nil;
                MaxAttVal[MaxAtt]     = 0;
                AttDefUses[MaxAtt]    = Nil;
                
                if (attribute.get_DataType() == ADT_Formula)    // IMPLICIT
                {
                    CAttributeFormula& attr = static_cast<CAttributeFormula&>(attribute);
                    _UNK.cval = UNKNOWN;
                    _NA.dval = NA;
                    //Buff = Alloc(attr.get_Value().size(), char);
                    Buff = strdupa(attr.get_Value().c_str());
                    PreviousError = false;
                    BN = 0;

                    /*  Allocate initial stack and attribute definition  */

                    TStack = Alloc(TStackSize=50, EltRec);
                    TSN = 0;

                    AttDef[MaxAtt] = Alloc(DefSize = 100, DefElt);
                    DN = 0;

                    /*  Parse Buff as an expression terminated by a period  */

                    Expression();
                    if ( ! Find((__String)".") ) DefSyntaxError((__String)"'.' ending definition");

                    if ( ! PreviousError )
                    {
                        if ( DN == 1 && DefOp(AttDef[MaxAtt][0]) == OP_ATT &&
                             strcmp(AttName[MaxAtt], "case weight") )
                        {
                            Error(SAMEATT, AttName[ (long) DefSVal(AttDef[MaxAtt][0]) ], Nil);
                        }

                        if ( TStack[0].Type == 'B' )
                        {
                            /*  Defined attributes should never have a value N/A  */


                            MaxAttVal[MaxAtt] = 3;
                            AttValName[MaxAtt] = AllocZero(4, __String);
                            AttValName[MaxAtt][1] = strdup("??");
                            AttValName[MaxAtt][2] = strdup("t");
                            AttValName[MaxAtt][3] = strdup("f");
                        }
                        else
                        {
                            MaxAttVal[MaxAtt] = 0;
                        }
                    }

                    if ( PreviousError )
                    {
                        DN = 0;
                        SpecialStatus[MaxAtt] = EXCLUDE;
                    }

                    /*  Write a terminating marker  */

                    DefOp(AttDef[MaxAtt][DN]) = OP_END;

                    Free(Buff);
                    Free(TStack);
                    
                    ListAttsUsed();

                }
                else
                {
                    int	ValCeiling=100, BaseYear;
                    time_t	clock;
                    if (attribute.get_DataType() == ADT_Continous)
                    {
                        
                    }
                    else
                    {
                        if (attribute.get_DataType() == ADT_Timestamp)
                        {
                            // cast
                            //CAttributeDateTime& attr = static_cast<CAttributeDateTime&>(attribute);
                            SpecialStatus[MaxAtt] = TSTMPVAL;
                            if ( ! TSBase )
                            {
                                clock = time(0);
                                BaseYear = gmtime(&clock)->tm_year + 1900;
                                SetTSBase(BaseYear);
                            }
                            
                        }
                        else if (attribute.get_DataType() == ADT_Date)
                        {
                            SpecialStatus[MaxAtt] = DATEVAL;
                        }
                        else if (attribute.get_DataType() == ADT_Time)
                        {
                            SpecialStatus[MaxAtt] = STIMEVAL;
                        }
                        else if (attribute.get_DataType() == ADT_Discrete)
                        {
                            SpecialStatus[MaxAtt] = DISCRETE;
                            CAttributeDiscrete& attr = static_cast<CAttributeDiscrete&>(attribute);
                            if (attr.get_MaxDiscreteItems() > 0)
                            {
                                AttValName[MaxAtt] = Alloc(attr.get_MaxDiscreteItems() + 3, __String);
                                AttValName[MaxAtt][0] = (char *) (long) attr.get_MaxDiscreteItems() + 1;
                                AttValName[MaxAtt][(MaxAttVal[MaxAtt]=1)] = strdup("N/A");
                            }
                            else if (attr.get_Value().size() > 0)
                            {
                                if (attr.get_IsOrdered())
                                    SpecialStatus[MaxAtt] = ORDERED;
                                
                        	AttValName[MaxAtt] = AllocZero(ValCeiling, __String);
                        	AttValName[MaxAtt][(MaxAttVal[MaxAtt]=1)] = strdup("N/A");
                                for (string& s : attr.get_Value())
                                {
                                    if ( ++MaxAttVal[MaxAtt] >= ValCeiling )
                                    {
                                        ValCeiling += 100;
                                        Realloc(AttValName[MaxAtt], ValCeiling, __String);
                                    }
                                    AttValName[MaxAtt][MaxAttVal[MaxAtt]] = strdup(s.c_str());
                                }
                                if ( Ordered(MaxAtt) && MaxAttVal[MaxAtt] <= 3 )
                                {
                                    SpecialStatus[MaxAtt] = 0;
                                }
                                if ( MaxAttVal[MaxAtt] > MaxDiscrVal ) 
                                    MaxDiscrVal = MaxAttVal[MaxAtt];
                            }
                        }
                        else if (attribute.get_DataType() == ADT_Ignore)
                        {
                            SpecialStatus[MaxAtt] = EXCLUDE;
                        }
                        else if (attribute.get_DataType() == ADT_Label)
                        {
                            LabelAtt = MaxAtt;
                            SpecialStatus[MaxAtt] = EXCLUDE;
                        }
                    }
                }
                    
                if (attribute.get_Name() == "case weight")
                {
                    CWtAtt = MaxAtt;
                    if (!attribute.get_DataType() == ADT_Continous)
                        throw seexception("case weight attribute must of continuous type.");
                    if ( Skip(CWtAtt) )
                    {
                        CWtAtt = 0;
                    }
                    else
                    {
                        SpecialStatus[CWtAtt] |= SKIP;
                    }
                }
                return S_Success;
            }
            catch(std::exception& ex)
            {
                return E_UnknownError;
            }
        }
    }
}
