using System;

namespace QReal.Utils
{
    public class StringHelper
    {
        /// <summary>
        /// Ensures that the string is surrounded with quotation marks, so that paths with 
        /// spaces in them do not cause an exception.
        /// </summary>
        /// <param name="urlToBeQuoted">The string to be quoted.</param>
        /// <returns>The original string surrounded with quotation marks</returns>
        public static string Quote(string stringToBeQuoted)
        {
            return String.Format(@"""{0}""", stringToBeQuoted);
        }

        public static string[] ParseLabel(string sourceLabel)
        {
            string[] labelParsed = sourceLabel.Split('.');
            if (labelParsed.Length != 4)
            {
                throw new Exception("Source label has invalid format " + sourceLabel);
            }
            return labelParsed;
        }
    }
}
