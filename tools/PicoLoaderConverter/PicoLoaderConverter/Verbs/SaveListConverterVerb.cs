using CommandLine;
using PicoLoaderConverter.SaveList;

namespace PicoLoaderConverter.Verbs;

[Verb("savelist", HelpText = "Convert save list from csv to bin.")]
sealed class SaveListConverterVerb : IConverterVerb
{
    [Option('i', Required = true, HelpText = "Input .csv file.")]
    public required string InputFile { get; init; }

    [Option('o', Required = true, HelpText = "Output .bin file.")]
    public required string OutputFile { get; init; }

    public void Run()
    {
        // Convert save list from csv to bin
        var factory = new SaveListFactory();
        var apList = factory.FromCsv(File.ReadAllText(InputFile));
        var binaryList = factory.ToBinary(apList);
        File.WriteAllBytes(OutputFile, binaryList);
    }
}
