<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="Default.aspx.cs" Inherits="BuildsVisualization._Default" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
    <title>QReal Builds</title>
</head>
<body>
    <form id="form1" runat="server">
        <div>
            <table> 
                <tr> 
                    <td>
                        <asp:TreeView ID="buildPlatformsTreeView" runat="server" ShowLines="True" 
                            onselectednodechanged="buildPlatformsTreeView_SelectedNodeChanged">
                        </asp:TreeView>
                    </td> 
                    <td width="10"/> 
                    <td valign="top">
                        <asp:GridView ID="buildsGridView" runat="server" AutoGenerateColumns="False">
                            <Columns>
                                <asp:BoundField HeaderText="Version" DataField="Version" />
                                <asp:BoundField HeaderText="Commit Message" DataField="Message" />
                                <asp:BoundField HeaderText="Autor" DataField="Autor" />
                                <asp:HyperLinkField HeaderText="Download" Text="Download" DataNavigateUrlFields="Url" />
                            </Columns>
                        </asp:GridView>
                    </td> 
                </tr> 
            </table>
        </div>
    </form>
</body>
</html>
