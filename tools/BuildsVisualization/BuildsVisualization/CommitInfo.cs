using System;
using System.Collections.Generic;
using System.Web;

namespace BuildsVisualization
{
    public class CommitInfo
    {
        protected string message;

        public string Message
        {
            get
            {
                return message;
            }
        }

        protected string autor;

        public string Autor
        {
            get
            {
                return autor;
            }
        }

        public CommitInfo(string message, string autor)
        {
            this.message = message;
            this.autor = autor;
        }

        public CommitInfo()
        {
        }
    }
}
