using System;
using System.Collections.Generic;
using System.Text;
using ThoughtWorks.CruiseControl.Core;
using Exortech.NetReflector;
using QReal.Utils;

namespace QReal.SvnRevisionLabeller
{
    [ReflectorType("svnRevisionLabeller")]
    public class SvnRevisionLabeller : VersionInfo, ILabeller
    {
        public string Generate(IIntegrationResult integrationResult)
        {
            try
            {
	            int rebuild = 0;
	            int revision = SvnProcessHelper.GetSvnRevision(SvnOptions);
	            IntegrationSummary lastIntegration = integrationResult.LastIntegration;
	            if ((integrationResult != null) && (!lastIntegration.IsInitial()))
	            {
	                string lastLabel = lastIntegration.Label;
	                string[] labelParsed = StringHelper.ParseLabel(lastLabel);
	                int lastRevision = Int32.Parse(labelParsed[2]);
	                if (lastRevision == revision)
	                {
	                    rebuild = Int32.Parse(labelParsed[3]) + 1;
	                }
	            }
	            string resultLabel = Major.ToString() + "." + Minor.ToString() + "." 
	                + revision.ToString() + "." + rebuild.ToString();
	            return resultLabel;
            }
            catch (Exception ex)
            {
                throw new Exception("Failed during svn revision labelling " + ex.ToString());
            }
        }

        public void Run(IIntegrationResult result)
        {
            // i don't really know if this method is called, but in source code of ccnet it is implemented like this
            result.Label = Generate(result);
        }
    }
}
