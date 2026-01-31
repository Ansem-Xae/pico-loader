using CommandLine;
using PicoLoaderConverter.Verbs;

namespace PicoLoaderConverter;

static class Program
{
    /// <summary>
    /// List of verbs supported by the Pico Loader Converter.
    /// To make a new verb, create a class implementing <see cref="IConverterVerb"/> and
    /// add it to this list.
    /// </summary>
    private static readonly Type[] sVerbs = [
        typeof(ApListConverterVerb),
        typeof(SaveListConverterVerb),
        typeof(PatchListConverterVerb)
    ];

    static void Main(string[] args)
    {
        Parser.Default
            .ParseArguments(args, sVerbs)
            .WithParsed<IConverterVerb>(o => o.Run());
    }
}
